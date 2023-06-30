

# ESP32-S2-Saola-1
ESP32-S2-WROVER is a powerful, generic Wi-Fi MCU module that integrates ESP32-S2. It has a PCB antenna, a 4 MB external SPI flash and an additional 2 MB PSRAM

# ESP32-S2-SOLO-N4R2
ESP32-S2R2 with 2 MB PSRAM inside, 4 MB SPI flash, PCB antenna

>Some PSRAM chips are 1.8 V devices and some are 3.3 V. The working voltage of the PSRAM chip must match the working voltage of the flash component. Consult the datasheet for your PSRAM chip and ESP32 device to find out the working voltages. For a 1.8 V PSRAM chip, make sure to either set the MTDI pin to a high signal level on bootup, or program ESP32 eFuses to always use the VDD_SIO level of 1.8 V. Not doing this can damage the PSRAM and/or flash chip


```sh

export ESPPORT=/dev/cu.usbserial-110
esptool.py --port $ESPPORT erase_flash

idf.py set-target esp32s2
idf.py fullclean

idf.py build flash monitor

idf.py save-defconfig     

openssl genrsa -out ./secrets/secure_boot_signing_key.pem 3072
idf.py bootloader

```

## Github Releases API

```sh
curl -L \
  -H "Accept: application/vnd.github+json" \
  -H "X-GitHub-Api-Version: 2022-11-28" \
  https://api.github.com/repos/stan-kondrat/esp-ota-github-example/releases

curl -L \
  -H "Accept: application/vnd.github+json" \
  -H "X-GitHub-Api-Version: 2022-11-28" \
  https://api.github.com/repos/stan-kondrat/esp-ota-github-example/releases/latest
```

## Logs

```sh
idf.py fullclean
idf.py build flash monitor > build.log 2>&1
```