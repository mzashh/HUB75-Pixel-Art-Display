# HUB75-Pixel-Art-Display
A  WIFI enabled LED Pixel Art Display which plays GIF files based on HUB75 panels driven by the ESP32

* The colours are much more vibrant in person, also the flickring only shows up on camera.

<p align="center">
<img src="https://github.com/mzashh/HUB75-Pixel-Art-Display/blob/main/images/display.jpg" width="500">
</p>

# Dev Branch Changes
Version 0.4.5a (Major Update)
  * Added an NTP Clock
  * Added Scrolling Custom Text
  * Clock and Text colour can be changed via the WebUI
  * Text Font and scroll speed can be changed
  * Added Toggle for GIF display, Clock and text.
  * Added Toggle for GIF Looping
    
Version 0.2.5a
  * Changed the File System to LittleFS.

# Features

  * Upon powering up the Panel connects to the WIFI network and shows the firmware version, IP address, RSSI and WIFI SSID for 4 sec.

  * It then plays the GIF files stored in the SPI Flash of the ESP32.

  * An NTP Clock Starts up and Shows the Time (configure GMT offset and DST offset in the firmware.

  * I have also implemented a WEB-UI using an Async web server which allows to upload, delete, download and play other GIF files on the ESP32.
  
  * You can also control the brightness via the slider.

  * The WEB-UI also has auth functionality and allows for remote rebooting of the ESP32.
  
  * Added support for 1/8 scan panels via modified library, use firmware_1-8 and modified library.

  * You can disable the Clock and insted add a custom scrolling text.

  * Text and clock colour can be changed via the WebUI.

  * Text scroll speed and Size can be changed via the WebUI.

  * Clock, Text or the GIF playback can be disabled or enabled individually.

  * GIF loop toggle allows to loop one GIF or play all GIFs which are stored in the flash one by one.
  
  
<p align="center">
<img src="https://github.com/mzashh/HUB75-Pixel-Art-Display/blob/main/images/WUI.png" width="1000" height="700">
</p>

# Hardware
* Any HUB75 panel compatible with the https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-DMA library 
* By default a 64x64 1/32 scan panel is supported.

* An ESP32, S2 or S3. 

# Schematics
* GPIO 34+ on the ESP32 are input only!!
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
* Use Arduino IDE v1.8.19 or Platform IO

* Required libraries

  https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-DMA
  
  https://github.com/bitbank2/AnimatedGIF

  https://github.com/mrcodetastic/GFX_Lite
  
  https://github.com/me-no-dev/ESPAsyncWebServer
  
  https://github.com/me-no-dev/AsyncTCP
  
* Modified library for 1/8 scan panels https://github.com/mzashh/ESP32-HUB75-MatrixPanel-DMA
  
* Default credentials
  
  * The WIFI SSID can be changed in firmware.ino(only use 2.4GHZ network as the ESP32 supports those only)
  
  * The WIFI password can be changed in firmware.ino
  
  * The WEB-UI username can be changed in firmware.ino
  
  * The WEB-UI password can be changed in firmware.ino
  
* Misc stuff
    
  * The default brightness can be changed in firmware.ino (value can be 0-255)
    
  * Other resolution Panels can be used as per the use cases shown in https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-DMA   

  * Auth functionality can be bypassed (disabled) by changing "false" in line 258 in webserver.ino to "true"


# Credits

  * Animated GIF example from https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-DMA
  
  * Async webserver base code from https://github.com/smford/esp32-asyncwebserver-fileupload-example
    
  
   
  

