# HUB75 Pixel Art Display

A Wi-Fi-enabled LED Pixel Art Display that plays GIF files on HUB75 panels, powered by an ESP32.

> **Note**: The colors are much more vibrant in person, and the flickering only appears on camera.

<p align="center">
<img src="https://github.com/mzashh/HUB75-Pixel-Art-Display/blob/main/images/display.jpg" width="500">
</p>

---

## Dev Branch Updates

### Version 0.4.5a (Major Update)
- Added an **NTP Clock**.
- Added **Scrolling Custom Text**.
- Clock and text colors can now be changed via the WebUI.
- Text font and scroll speed are configurable.
- Added toggles for:
  - GIF display
  - Clock
  - Scrolling text
- Added a toggle for GIF looping.

### Version 0.2.5a
- Switched the file system to **LittleFS**.

---

## Features

- **Wi-Fi Connectivity**: On startup, the panel connects to the Wi-Fi network and displays the firmware version, IP address, RSSI, and Wi-Fi SSID for 4 seconds.
- **GIF Playback**: Plays GIF files stored in the ESP32's SPI Flash.
- **NTP Clock**: Displays the current time (configurable GMT and DST offsets in the firmware).
- **WebUI**:
  - Upload, delete, download, and play GIF files.
  - Control brightness via a slider.
  - Authentication functionality for secure access.
  - Remote rebooting of the ESP32.
- **Custom Scrolling Text**: Replace the clock with custom scrolling text.
- **WebUI Customization**:
  - Change text and clock colors.
  - Adjust text scroll speed and size.
- **Playback Toggles**:
  - Enable or disable GIF playback, clock, or scrolling text individually.
  - Toggle GIF looping to either loop a single GIF or play all stored GIFs sequentially.
- **1/8 Scan Panel Support**: Added support for 1/8 scan panels via a modified library (use `firmware_1-8` and the modified library).

<p align="center">
<img src="https://github.com/mzashh/HUB75-Pixel-Art-Display/blob/main/images/WUI.png" width="1000" height="700">
</p>

---

## Planned Features

> These features are tentative and may not be implemented due to time constraints or motivation.

- Proper date system
- Weather display
- Additional clock types and faces
- Canvas drawing functionality
- Stopwatch/Timer/Pomodoro modes

---

## Hardware Requirements

- **HUB75 Panel**: Compatible with the [ESP32-HUB75-MatrixPanel-DMA library](https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-DMA). By default, a 64x64 1/32 scan panel is supported.
- **ESP32 Board**: ESP32, ESP32-S2, or ESP32-S3.

### Pin Configuration

> **Note**: GPIO 34+ on the ESP32 are input-only. GPIO 5, 23, 18, and 19 are reserved for future SD card support.

| Panel | ESP32 GPIO Pin |
|--------|----------|
| R1     | 25       |
| G1     | 26       |
| B1     | 27       |
| R2     | 14       |
| G2     | 12       |
| B2     | 13       |
| A      | 22       |
| B      | 32       |
| C      | 33       |
| D      | 17       |
| E      | 21       |
| OE     | 15       |
| CLK    | 16       |
| LAT    | 4        |

---

## Firmware

### Development Tools
- **Arduino IDE** (v1.8.19) or **PlatformIO**.

### Required Libraries
- [ESP32-HUB75-MatrixPanel-DMA](https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-DMA)
- [AnimatedGIF](https://github.com/bitbank2/AnimatedGIF)
- [GFX_Lite](https://github.com/mrcodetastic/GFX_Lite)
- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
- [AsyncTCP](https://github.com/me-no-dev/AsyncTCP)

### Modified Library for 1/8 Scan Panels
- [ESP32-HUB75-MatrixPanel-DMA (Modified)](https://github.com/mzashh/ESP32-HUB75-MatrixPanel-DMA)

### Configuration
- **Wi-Fi**:
  - Change the Wi-Fi SSID and password in `firmware.ino` (only 2.4GHz networks are supported).
- **WebUI**:
  - Update the WebUI username and password in `firmware.ino`.
- **Brightness**:
  - Set the default brightness in `firmware.ino` (range: 0–255).
- **Panel Resolution**:
  - Use other resolution panels as per the examples in the [ESP32-HUB75-MatrixPanel-DMA library](https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-DMA).
- **Authentication**:
  - Disable authentication by changing `false` to `true` on line 258 in `webserver.ino`.

---

## Credits

- **GIF Playback**: Based on examples from the [ESP32-HUB75-MatrixPanel-DMA library](https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-DMA).
- **WebUI**: Inspired by the [ESP32 Async WebServer File Upload Example](https://github.com/smford/esp32-asyncwebserver-fileupload-example).
