
#include "mac.h"


uint8_t mac_eq(const uint8_t *m1, const uint8_t *m2)
{
    for (int i = 0; i < 6; i++) {
        if (m1[i] != m2[i]) {
            return 0;
        }
    }
    return 1;
}

