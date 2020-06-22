
#include <arpa/inet.h>
#include <errno.h>
#include <linux/filter.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>


#define DATA_RATE_1_MBPS  0x02
#define DATA_RATE_2_MBPS  0x04
#define DATA_RATE_6_MBPS  0x0c
#define DATA_RATE_9_MBPS  0x12
#define DATA_RATE_12_MBPS 0x18
#define DATA_RATE_18_MBPS 0x24
#define DATA_RATE_24_MBPS 0x30
#define DATA_RATE_36_MBPS 0x48
#define DATA_RATE_48_MBPS 0x60
#define DATA_RATE_54_MBPS 0x6c


#define CHANNEL_1  2412
#define CHANNEL_2  2417
#define CHANNEL_3  2422
#define CHANNEL_4  2427
#define CHANNEL_5  2432
#define CHANNEL_6  2437
#define CHANNEL_7  2442
#define CHANNEL_8  2447
#define CHANNEL_9  2452
#define CHANNEL_10 2457
#define CHANNEL_11 2462
#define CHANNEL_12 2467
#define CHANNEL_13 2472


#define MAX_MESSAGE_SIZE 512
#define WLAN_LEN 24
#define ACTIONFRAME_HEADER_LEN 8
#define VENDORSPECIFIC_CONTENT_LEN 7


#define PACKET_FILTER_LENGTH 20
static struct sock_filter packet_filter_code[PACKET_FILTER_LENGTH] =
{
    { 0x30, 0, 0, 0x00000003 },  // ldb [3]                      // radiotap header length : MS byte
    { 0x64, 0, 0, 0x00000008 },  // lsh #8                       // left shift it
    { 0x7, 0, 0, 0x00000000 },   // tax                          // 'store' it in X register
    { 0x30, 0, 0, 0x00000002 },  // ldb [2]                      // radiotap header length : LS byte
    { 0x4c, 0, 0, 0x00000000 },  // or  x                        // combine A & X to get radiotap header length in A
    { 0x7, 0, 0, 0x00000000 },   // tax                          // 'store' it in X
    { 0x50, 0, 0, 0x00000000 },  // ldb [x + 0]                  // right after radiotap header is the type and subtype
    { 0x54, 0, 0, 0x000000fc },  // and #0xfc                    // mask the interesting bits, a.k.a 0b1111 1100
    { 0x15, 0, 10, 0x000000d0 }, // jeq #0xd0 jt 9 jf 19         // compare the types (0) and subtypes (0xd)
    { 0x40, 0, 0, 0x00000018 },  // Ld  [x + 24]                 // 24 bytes after radiotap header is the end of MAC header,
                                                                 // so it is category and OUI (for action frame layer)
    { 0x15, 0, 8, 0x7f18fe34 },  // jeq #0x7f18fe34 jt 11 jf 19  // compare with category = 127 (Vendor specific) and OUI 18:fe:34
    { 0x50, 0, 0, 0x00000020 },  // ldb [x + 32]                 // begining of Vendor specific content + 4 ?random? bytes : element id
    { 0x15, 0, 6, 0x000000dd },  // jeq #0xdd jt 13 jf 19        // element id should be 221 (according to the doc)
    { 0x40, 0, 0, 0x00000021 },  // Ld  [x + 33]                 // OUI (again!) on 3 LS bytes
    { 0x54, 0, 0, 0x00ffffff },  // and #0xffffff                // Mask the 3 LS bytes
    { 0x15, 0, 3, 0x0018fe34 },  // jeq #0x18fe34 jt 16 jf 19    // Compare with OUI 18:fe:34
    { 0x50, 0, 0, 0x00000025 },  // ldb [x + 37]                 // Type
    { 0x15, 0, 1, 0x00000004 },  // jeq #0x4 jt 18 jf 19         // Compare type with type 0x4 (corresponding to ESP_NOW)
    { 0x6, 0, 0, 0x00040000 },   // ret #262144                  // return 'True'
    { 0x6, 0, 0, 0x00000000 },   // ret #0                       // return 'False'
};
static struct sock_fprog packet_filter = {PACKET_FILTER_LENGTH, packet_filter_code};


void callback(uint8_t mac[6], uint8_t *payload, int len)
{
    char macstr[18];
    snprintf(macstr, sizeof(macstr), "%02x:%02x:%02x:%02x:%02x:%02x",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    char payloadstr[len];
    snprintf(payloadstr, sizeof(payloadstr), "%s", (char *) payload);

    printf("%s => %s\n", macstr, payloadstr);

    struct timeval now;
    gettimeofday(&now, NULL);
    unsigned long timestamp_usec = (now.tv_sec * 1000000) + now.tv_usec;

    char filename[50];
    snprintf(filename, sizeof(filename), "/opt/espnow/%lu.json", timestamp_usec);

    FILE *file = fopen(filename, "w+");
    fprintf(file, "%s => %s\n", macstr, payloadstr);
    fclose(file);
}

int main(int argc, char **argv)
{
    printf("Running\n");
    const char *interface = "wlan0";
    int socket_priority = 7;

    struct sockaddr_ll dest_addr;
    struct ifreq ifreq;

    int socket_descriptor = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (socket_descriptor < 0)
    {
        printf("socket failed: %d\n", errno);
        return 1;
    }

    strncpy((char *) ifreq.ifr_name, interface, IFNAMSIZ);
    if (ioctl(socket_descriptor, SIOCGIFINDEX, &ifreq) < 0)
    {
        printf("ioctl failed: %d\n", errno);
        return 1;
    }

    dest_addr.sll_family = PF_PACKET;
    dest_addr.sll_protocol = htons(ETH_P_ALL);
    dest_addr.sll_ifindex = ifreq.ifr_ifindex;

    if (bind(socket_descriptor, (struct sockaddr *) &dest_addr, sizeof(dest_addr)) < 0)
    {
        printf("bind failed: %d\n", errno);
        return 1;
    }

    if (setsockopt(socket_descriptor, SOL_SOCKET, SO_PRIORITY, &socket_priority, sizeof(socket_priority)) < 0)
    {
        printf("setsockopt failed setting priority: %d\n", errno);
        return 1;
    }

    if (setsockopt(socket_descriptor, SOL_SOCKET, SO_ATTACH_FILTER, &packet_filter, sizeof(packet_filter)) < 0)
    {
        printf("setsockopt failed attaching filter: %d\n", errno);
        return 1;
    }


    uint8_t raw_bytes[MAX_MESSAGE_SIZE];
    while (1)
    {
        int raw_bytes_len = recvfrom(socket_descriptor, raw_bytes, MAX_MESSAGE_SIZE, MSG_TRUNC, NULL, 0);
        if (raw_bytes_len < 0)
        {
            printf("recvfrom failed: %d\n", errno);
            return 1;
        }
        else
        {
            int radio_tap_len = raw_bytes[2] + (raw_bytes[3] << 8);
            //if (radio_tap_len < 128)
            //{
                uint8_t *mac = raw_bytes + radio_tap_len + 10;
                uint8_t *payload = raw_bytes + radio_tap_len + WLAN_LEN + ACTIONFRAME_HEADER_LEN + VENDORSPECIFIC_CONTENT_LEN;
                int payload_len = raw_bytes[radio_tap_len + WLAN_LEN + ACTIONFRAME_HEADER_LEN + 1] - 5;
                if (mac != NULL && payload != NULL && payload_len > 0)
                {
                    callback(mac, payload, payload_len);
                }
                //else
                //{
                    //printf("ignoring invalid message\n");
                //}
            //}
            //else
            //{
                //printf("ignoring invalid message (radio tap too long)\n");
            //}
        }
    }

    printf("Done\n");
    return 0;
}

