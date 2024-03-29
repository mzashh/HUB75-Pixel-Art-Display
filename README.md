# HUB75-Pixel-Art-Display
A  WIFI enabled LED Pixel Art Display which plays GIF files based on HUB75 panels driven by the ESP32

* The colours are much more vibrant in person, also the flickring only shows up on camera.

<p align="center">
<img src="https://github.com/mzashh/HUB75-Pixel-Art-Display/blob/main/images/display.jpg" width="500">
</p>

# Features
* As of now the firmware has minimum fuctionality.

  * Upon powering up the Panel connects to the WIFI network and shows the firmware version, IP address, RSSI and WIFI SSID for 4 sec.

  * It then plays the GIF files stored in the SPI Flash of the ESP32.

  * I have also implemented a WEB-UI using an Async web server which allows to upload, delete, download and play other GIF files on the ESP32.
  
  * You can also control the brightness via the slider.

  * The WEB-UI also has auth functionality and allows for remote rebooting of the ESP32.
  
  * Added support for 1/8 scan panels via modified library, use firmware_1-8 and modified library.
  
  
<p align="center">
<img src="https://github.com/mzashh/HUB75-Pixel-Art-Display/blob/main/images/WUI.png" width="450">
</p>

# Hardware
* Any HUB75 panel compatible with the https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-DMA library 
* By default a 64x64 1/32 scan panel is supported.
* For firmware_1-8, by default a 32x32 1/8 scan outdoor panel is supported.

* An ESP32, S2 or S3. 

# Schematics
* GPIO 34+ are on the ESP32 are input only!!
* GPIO 5, 23, 18, 19 are reserverd for a SD card which will be added in the future.

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
* LAT_PIN --> 4

# Firmware
* Use Arduino IDE v1.8.19

* Required libraries

  https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-DMA
  
  https://github.com/bitbank2/AnimatedGIF
  
  https://github.com/me-no-dev/ESPAsyncWebServer
  
  https://github.com/me-no-dev/AsyncTCP
  
* Modified library for 1/8 scan panels https://github.com/mzashh/ESP32-HUB75-MatrixPanel-DMA
  
* Default credentials
  
  * The WIFI SSID can be changed in firmware.ino line 34 (only use 2.4GHZ network as the ESP32 supports those only)
  
  * The WIFI password can be changed in firmware.ino line 35
  
  * The WEB-UI username can be changed in firmware.ino line 36
  
  * The WEB-UI password can be changed in firmware.ino line 37
  
* Misc stuff
    
  * The default brightness can be changed in firmware.ino line 42 (value can be 0-255)
    
  * If you want the Panel to play all stored GIFS one by one instead of just looping a particular one then in firmware.ino uncomment lines 410-412
    
  * If you want the Panel to wait until WIFI is connected in firmware.ino uncomment lines 334-336
    
  * Other resolution Panels can be used as per the use cases shown in https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-DMA   

  * Auth functionality can be bypassed (disabled) by changing "false" in line 169 in webserver.ino to "true"


# Credits

  * Animated GIF example from https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-DMA
  
  * Async webserver base code from https://github.com/smford/esp32-asyncwebserver-fileupload-example
    
  
   
  

