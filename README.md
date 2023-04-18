# HUB75-Pixel-Art-Display
A LED Pixel Art Display based on HUB75 panels driven by the ESP32

# Features
As of now the firmware has minimum fuctionality, it plays a GIF files stored in the SPI Flash of the ESP32.

I have also implemented a WEB-UI using a async web server which allows to upload, delete, download and delete GIFS on the ESP32.

The WEB-UI also has auth functionality and allows for remote rebooting of the ESP32.

![alt text](https://github.com/mzashh/HUB75-Pixel-Art-Display/blob/6dd2d68b21905c8d801fee8adb18e6ca6addbe45/images/UI.png)

# Hardware
* Any HUB75 panel compatible with the https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-DMA library

* An ESP32, S2 or S3 

# Schematics
* GPIO 34+ are on the ESP32 are input only!!
* GPIO 5, 23, 18, 19 are reserverd for a SD card which will be added in the future

* R1_PIN --> 25
* G1_PIN --> 26
* B1_PIN --> 27
* R2_PIN --> 14
* G2_PIN --> 12
* B2_PIN --> 13
* A_PIN --> 22
* B_PIN --> 32
* C_PIN --> 33
* D_PIN --> 17
* E_PIN --> 21
* OE_PIN --> 15
* CLK_PIN --> 16

# Firmware
* Required libraries

  https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-DMA
  https://github.com/bitbank2/AnimatedGIF
  https://github.com/me-no-dev/ESPAsyncWebServer
  https://github.com/me-no-dev/AsyncTCP

