

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

```



# Logs

➜  hello git:(main) idf.py build flash monitor
Last login: Sat Jun 17 17:00:47 on ttys004
Executing action: all (aliases: build)
Running ninja in directory /Users/user/projects/esp32-secure-hello/hello/build
Executing "ninja all"...
[1/4] cd /Users/user/projects/esp32-secure-hello/hello/build/esp-idf/esptool...tion-table.bin /Users/user/projects/esp32-secure-hello/hello/build/hello.binhello.bin binary size 0x2f020 bytes. Smallest app partition is 0x100000 bytes. 0xd0fe0 bytes (82%) free.
[1/1] cd /Users/user/projects/esp32-secure-hello/hello/build/bootloader/esp-...Users/user/projects/esp32-secure-hello/hello/build/bootloader/bootloader.binBootloader binary size 0x5430 bytes. 0x1bd0 bytes (25%) free.
[4/4] Completed 'bootloader'Executing action: flash
Running ninja in directory /Users/user/projects/esp32-secure-hello/hello/build
Executing "ninja flash"...
[1/5] cd /Users/user/projects/esp32-secure-hello/hello/build/esp-idf/esptool_py && /Users/user/.espressif/python_env/idf5.0_py3.11_env/bin/python /Users/user/esp/esp-idf-v5.0.1/components/partition_table/check_sizes.py --offset 0x8000 partition --type app /Users/user/projects/esp32-secure-hello/hello/build/partition_table/partition-table.bin /Users/user/projects/esp32-secure-hello/hello/build/hello.bin
hello.bin binary size 0x2f020 bytes. Smallest app partition is 0x100000 bytes. 0xd0fe0 bytes (82%) free.
[2/5] Performing build step for 'bootloader'
[1/1] cd /Users/user/projects/esp32-secure-hello/hello/build/bootloader/esp-idf/esptool_py && /Users/user/.espressif/python_env/idf5.0_py3.11_env/bin/python /Users/user/esp/esp-idf-v5.0.1/components/partition_table/check_sizes.py --offset 0x8000 bootloader 0x1000 /Users/user/projects/esp32-secure-hello/hello/build/bootloader/bootloader.bin
Bootloader binary size 0x5430 bytes. 0x1bd0 bytes (25%) free.
[3/5] No install step for 'bootloader'
[4/5] Completed 'bootloader'
[4/5] cd /Users/user/esp/esp-idf-v5.0.1/components/esptool_py && /opt/homebrew/Cellar/cmake/3.25.3/bin/cmake -D IDF_PATH=/Users/user/esp/esp-idf-v5.0.1 -D "SERIAL_TOOL=/Users/user/.espressif/python_env/idf5.0_py3.11_env/bin/python;;/Users/user/esp/esp-idf-v5.0.1/components/esptool_py/esptool/esptool.py;--chip;esp32s2" -D "SERIAL_TOOL_ARGS=--before=default_reset;--after=hard_reset;write_flash;@flash_args" -D WORKING_DIRECTORY=/Users/user/projects/esp32-secure-hello/hello/build -P /Users/user/esp/esp-idf-v5.0.1/components/esptool_py/run_serial_tool.cmake
esptool esp32s2 -p /dev/cu.usbserial-110 -b 460800 --before=default_reset --after=hard_reset write_flash --flash_mode dio --flash_freq 80m --flash_size 4MB 0x1000 bootloader/bootloader.bin 0x10000 hello.bin 0x8000 partition_table/partition-table.bin
esptool.py v4.5.1
Serial port /dev/cu.usbserial-110
Connecting....
Chip is ESP32-S2 (revision v0.0)
Features: WiFi, No Embedded Flash, No Embedded PSRAM, ADC and temperature sensor calibration in BLK2 of efuse V1
Crystal is 40MHz
MAC: 7c:df:a1:00:7d:e2
Uploading stub...
Running stub...
Stub running...
Changing baud rate to 460800
Changed.
Configuring flash size...
Flash will be erased from 0x00001000 to 0x00006fff...
Flash will be erased from 0x00010000 to 0x0003ffff...
Flash will be erased from 0x00008000 to 0x00008fff...
Compressed 21552 bytes to 13577...
Writing at 0x00001000... (100 %)
Wrote 21552 bytes (13577 compressed) at 0x00001000 in 0.6 seconds (effective 271.5 kbit/s)...
Hash of data verified.
Compressed 192544 bytes to 107949...
Writing at 0x00010000... (14 %)
Writing at 0x0001bdb9... (28 %)
Writing at 0x00021690... (42 %)
Writing at 0x00027369... (57 %)
Writing at 0x0002cc45... (71 %)
Writing at 0x00034b72... (85 %)
Writing at 0x0003b311... (100 %)
Wrote 192544 bytes (107949 compressed) at 0x00010000 in 2.9 seconds (effective 537.5 kbit/s)...
Hash of data verified.
Compressed 3072 bytes to 103...
Writing at 0x00008000... (100 %)
Wrote 3072 bytes (103 compressed) at 0x00008000 in 0.1 seconds (effective 221.6 kbit/s)...
Hash of data verified.

Leaving...
Hard resetting via RTS pin...
Executing action: monitor
Running idf_monitor in directory /Users/user/projects/esp32-secure-hello/hello
Executing "/Users/user/.espressif/python_env/idf5.0_py3.11_env/bin/python /Users/user/esp/esp-idf/tools/idf_monitor.py -p /dev/cu.usbserial-110 -b 115200 --toolchain-prefix xtensa-esp32s2-elf- --target esp32s2 /Users/user/projects/esp32-secure-hello/hello/build/hello.elf -m '/Users/user/.espressif/python_env/idf5.0_py3.11_env/bin/python' '/Users/user/esp/esp-idf/tools/idf.py'"...
--- idf_monitor on /dev/cu.usbserial-110 115200 ---
--- Quit: Ctrl+] | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H ---
ESP-ROM:esp32s2-rc4-20191025
Build:Oct 25 2019
rst:0x1 (POWERON),boot:0x8 (SPI_FAST_FLASH_BOOT)
SPIWP:0xee
mode:DIO, clock div:1
load:0x3ffe6108,len:0x1760
load:0x4004c000,len:0xabc
load:0x40050000,len:0x31b4
entry 0x4004c1c0
I (21) boot: ESP-IDF v5.0.1 2nd stage bootloader
I (21) boot: compile time 16:12:08
I (21) boot: chip revision: v0.0
I (24) boot.esp32s2: SPI Speed      : 80MHz
I (29) boot.esp32s2: SPI Mode       : DIO
I (33) boot.esp32s2: SPI Flash Size : 4MB
I (38) boot: Enabling RNG early entropy source...
I (44) boot: Partition Table:
I (47) boot: ## Label            Usage          Type ST Offset   Length
I (54) boot:  0 nvs              WiFi data        01 02 00009000 00006000
I (62) boot:  1 phy_init         RF data          01 01 0000f000 00001000
I (69) boot:  2 factory          factory app      00 00 00010000 00100000
I (77) boot: End of partition table
I (81) esp_image: segment 0: paddr=00010020 vaddr=3f000020 size=09a28h ( 39464) map
I (97) esp_image: segment 1: paddr=00019a50 vaddr=3ffc0b00 size=01a64h (  6756) load
I (99) esp_image: segment 2: paddr=0001b4bc vaddr=40024000 size=04b5ch ( 19292) load
I (111) esp_image: segment 3: paddr=00020020 vaddr=40080020 size=17024h ( 94244) map
I (133) esp_image: segment 4: paddr=0003704c vaddr=40028b5c size=07fa4h ( 32676) load
I (148) boot: Loaded app from partition at offset 0x10000
I (148) boot: Disabling RNG early entropy source...
I (160) cache: Instruction cache 	: size 8KB, 4Ways, cache line size 32Byte
I (160) cache: Data cache 		: size 8KB, 4Ways, cache line size 32Byte
I (163) esp_psram: Found 2MB PSRAM device
I (168) esp_psram: Speed: 40MHz
I (172) cpu_start: Pro cpu up.
I (586) esp_psram: SPI SRAM memory test OK
I (600) cpu_start: Pro cpu start user code
I (600) cpu_start: cpu freq: 160000000 Hz
I (600) cpu_start: Application information:
I (603) cpu_start: Project name:     hello
I (608) cpu_start: App version:      d2f4e2d
I (613) cpu_start: Compile time:     Jun 19 2023 16:12:04
I (619) cpu_start: ELF file SHA256:  b2de769ca87cf2a0...
I (625) cpu_start: ESP-IDF:          v5.0.1
I (630) cpu_start: Min chip rev:     v0.0
I (634) cpu_start: Max chip rev:     v1.99 
I (639) cpu_start: Chip rev:         v0.0
I (644) heap_init: Initializing. RAM available for dynamic allocation:
I (651) heap_init: At 3FFC2ED0 len 00039130 (228 KiB): DRAM
I (657) heap_init: At 3FFFC000 len 00003A10 (14 KiB): DRAM
I (664) heap_init: At 3FF9E000 len 00002000 (8 KiB): RTCRAM
I (670) esp_psram: Adding pool of 2048K of PSRAM memory to heap allocator
I (678) spi_flash: detected chip: generic
I (682) spi_flash: flash io: dio
I (686) cpu_start: Starting scheduler on PRO CPU.
I (691) esp_psram: Reserving pool of 32K of internal memory for DMA/internal allocations
I (691) example: Example configured to blink addressable LED!
I (701) gpio: GPIO[18]| InputEn: 0| OutputEn: 1| OpenDrain: 0| Pullup: 1| Pulldown: 0| Intr:0 
I (711) example: Turning the LED OFF!
I (1711) example: Turning the LED ON!

Done
➜  hello git:(main) 
