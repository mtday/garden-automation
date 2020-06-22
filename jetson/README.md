# Jetson Nano Configuration

* Download [JetPack](https://developer.nvidia.com/embedded/jetpack#install).
* Use Balena Etcher to flash the image onto an SD card.
* Insert SD card into Jetson Nano.
* Insert WiFi USB module.
* Connect a cat5 ethernet to home network.
* Connect the micro-USB cable to laptop (Mac OS).
* Use a jumper to short the J48 pins on the device to support powering from the adapter instead of a USB cable. A USB cable does not provide sufficient power to keep the device powered on during boot.
* Power on the device with the barrel connector power supply.
* Connect to the device over the USB cable:

```
screen /dev/tty.usbmodem* 115200
```


* Follow the boot instructions (accept terms of use, configure locale, create user, connect to wifi, etc.):

    * Use `mday` as the username
    * Use `wlan0` as the primary network interface
    * Use `jetson` as the hostname

* After initial configuration, disconnect the USB serial cable and SSH into the device.
* Update `/etc/sudoers` to allow sudo without a password:

```
printf "\nmday ALL=(ALL) NOPASSWD:ALL\n" | sudo tee -a /etc/sudoers
```

* Fix WiFi disconnects (instructions from [here](https://www.datatobiz.com/2019/10/03/fixing-wifi-connectivity-nvidia-jetson-nano/)):

```
sudo apt install -y git linux-headers-generic build-essential dkms
git clone https://github.com/pvaret/rtl8192cu-fixes
sudo dkms add ./rtl8192cu-fixes
sudo dkms install 8192cu/1.11
sudo depmod -a
sudo cp ./rtl8192cu-fixes/blacklist-native-rtl8192.conf /etc/modprobe.d/
sudo echo options rtl8xxxu ht40_2g=1 dma_aggregation=1 | sudo tee /etc/modprobe.d/rtl8xxxu.conf
sudo iw dev wlan0 set power_save off
rm -rf rtl8192cu-fixes
```

* Reboot the device:

```
sudo reboot now
```


* Disable login message:

```
sudo chmod 600 /etc/update-motd.d/*
```

* Add `~/.ssh/authorized_keys` file containing a public SSH key to allow passwordless SSH into the device.
* Add `~/.ssh/id_rsa` and `~/.ssh/id_rsa.pub` files for SSH off the device.
* Add `~/.gitconfig` for common git aliases and user info.
* Add `~/.vimrc` for common vim settings.
* Add `~/.inputrc` for common command-line/shell settings.
* Update the device:

```
sudo apt update -y && sudo apt upgrade -y
sudo apt autoremove -y
```

* Edit `/etc/gdm3/custom.conf` and uncomment/set `AutomaticLoginEnable = true` and `AutomaticLogin = mday`.

* Clone the garden automation source code:

```
git clone git@github.com:mtday/garden-automation.git
```

* Install common utilities:

```
sudo apt install -y apt-utils curl locate
```

* Install and configure Chrony NTP service.

```
sudo apt install -y chrony
sudo systemctl enable chrony.service
sudo systemctl start chrony.service
```

* Install and configure Mosquitto MQTT server.

```
sudo apt install -y mosquitto mosquitto-clients
sudo systemctl enable mosquitto.service
sudo systemctl start mosquitto.service
```

* Install and configure Node-RED:

```
bash <(curl -sL https://raw.githubusercontent.com/node-red/linux-installers/master/deb/update-nodejs-and-nodered)
sudo systemctl enable nodered.service
sudo systemctl start nodered.service
```

* Install and configure PostgreSQL:

```
sudo apt install -y postgresql
sudo systemctl enable postgresql.service
sudo systemctl start postgresql.service
echo "CREATE DATABASE automation;" | sudo -u postgres psql
echo "CREATE USER automation WITH ENCRYPTED PASSWORD 'automation';" | sudo -u postgres psql
echo "GRANT ALL PRIVILEGES ON DATABASE automation TO automation;" | sudo -u postgres psql
cat garden-automation/db/db.sql | psql -h localhost -U automation -W automation
```



