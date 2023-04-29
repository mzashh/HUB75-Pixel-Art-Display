//By mzashh https://github.com/mzashh
//For this version use this modified library https://github.com/mzashh/ESP32-HUB75-MatrixPanel-DMA

#define FILESYSTEM SPIFFS
#include <SPIFFS.h>
#include <AnimatedGIF.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include "ESP32-VirtualMatrixPanel-I2S-DMA.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include "webpages.h"

#define FIRMWARE_VERSION "v0.2.2a"

#define A_PIN   22 
#define B_PIN   32 
#define C_PIN   33


#define PANEL_RES_X 32      
#define PANEL_RES_Y 32     
#define PANEL_CHAIN 1  
#define NUM_ROWS 1 // Number of rows of chained INDIVIDUAL PANELS
#define NUM_COLS 1 // Number of INDIVIDUAL PANELS per ROW
#define SERPENT true
#define TOPDOWN false 

MatrixPanel_I2S_DMA *dma_display = nullptr;

// placeholder for the virtual display object
VirtualMatrixPanel  *OneEightMatrixDisplay = nullptr;
  
uint16_t myBLACK = dma_display->color565(0, 0, 0);
uint16_t myWHITE = dma_display->color565(255, 255, 255);
uint16_t myRED = dma_display->color565(255, 0, 0);
uint16_t myGREEN = dma_display->color565(0, 255, 0);
uint16_t myBLUE = dma_display->color565(0, 0, 255);

const String default_ssid = "yourssid";
const String default_wifipassword = "yourpassword";
const String default_httpuser = "admin";
const String default_httppassword = "admin";
const int default_webserverporthttp = 80;

const char* PARAM_INPUT = "value";
String inputMessage;
String sliderValue = "100"; //default brightness value

struct Config {
String ssid;               // wifi ssid
String wifipassword;       // wifi password
String httpuser;           // username to access web admin
String httppassword;       // password to access web admin
int webserverporthttp;     // http port number for web admin
};


 Config config;                        // configuration
 bool shouldReboot = false;            // schedule a reboot
 AsyncWebServer *server;               // initialise webserver

// function defaults
 String listFiles(bool ishtml = false);

 AnimatedGIF gif;
 File f;
 int x_offset, y_offset;



void GIFDraw(GIFDRAW *pDraw)
 {
    dma_display->setBrightness8(sliderValue.toInt());
    uint8_t *s;
    uint16_t *d, *usPalette, usTemp[320];
    int x, y, iWidth;

  iWidth = pDraw->iWidth;
  if (iWidth > MATRIX_WIDTH)
      iWidth = MATRIX_WIDTH;

    usPalette = pDraw->pPalette;
    y = pDraw->iY + pDraw->y; // current line
    
    s = pDraw->pPixels;
    if (pDraw->ucDisposalMethod == 2) // restore to background color
    {
      for (x=0; x<iWidth; x++)
      {
        if (s[x] == pDraw->ucTransparent)
           s[x] = pDraw->ucBackground;
      }
      pDraw->ucHasTransparency = 0;
    }
    if (pDraw->ucHasTransparency) // if transparency used
    {
      uint8_t *pEnd, c, ucTransparent = pDraw->ucTransparent;
      int x, iCount;
      pEnd = s + pDraw->iWidth;
      x = 0;
      iCount = 0; // count non-transparent pixels
      while(x < pDraw->iWidth)
      {
        c = ucTransparent-1;
        d = usTemp;
        while (c != ucTransparent && s < pEnd)
        {
          c = *s++;
          if (c == ucTransparent) // done, stop
          {
            s--; // back up to treat it like transparent
          }
          else // opaque
          {
             *d++ = usPalette[c];
             iCount++;
          }
        } // while looking for opaque pixels
        if (iCount) // any opaque pixels?
        {
          for(int xOffset = 0; xOffset < iCount; xOffset++ ){
          OneEightMatrixDisplay->drawPixel(x + xOffset, y, usTemp[xOffset]); // 565 Color Format
          }
          x += iCount;
          iCount = 0;
        }
        // no, look for a run of transparent pixels
        c = ucTransparent;
        while (c == ucTransparent && s < pEnd)
        {
          c = *s++;
          if (c == ucTransparent)
             iCount++;
          else
             s--; 
        }
        if (iCount)
        {
          x += iCount; // skip these
          iCount = 0;
        }
      }
    }
    else // does not have transparency
    {
      s = pDraw->pPixels;
      // Translate the 8-bit pixels through the RGB565 palette (already byte reversed)
      for (x=0; x<pDraw->iWidth; x++)
      {
       OneEightMatrixDisplay->drawPixel(x, y, usPalette[*s++]); // color 565
      }
    }
 } /* GIFDraw() */


void * GIFOpenFile(const char *fname, int32_t *pSize)
 {
  Serial.print("Playing gif: ");
  Serial.println(fname);
  f = FILESYSTEM.open(fname);
  if (f)
  {
    *pSize = f.size();
    return (void *)&f;
  }
  return NULL;
 } /* GIFOpenFile() */


void GIFCloseFile(void *pHandle)
 {
  File *f = static_cast<File *>(pHandle);
  if (f != NULL)
     f->close();
 } /* GIFCloseFile() */

 int32_t GIFReadFile(GIFFILE *pFile, uint8_t *pBuf, int32_t iLen)
 {
    int32_t iBytesRead;
    iBytesRead = iLen;
    File *f = static_cast<File *>(pFile->fHandle);
    // Note: If you read a file all the way to the last byte, seek() stops working
    if ((pFile->iSize - pFile->iPos) < iLen)
       iBytesRead = pFile->iSize - pFile->iPos - 1; // <-- ugly work-around
    if (iBytesRead <= 0)
       return 0;
    iBytesRead = (int32_t)f->read(pBuf, iBytesRead);
    pFile->iPos = f->position();
    return iBytesRead;
 } /* GIFReadFile() */

 int32_t GIFSeekFile(GIFFILE *pFile, int32_t iPosition)
 { 
  int i = micros();
  File *f = static_cast<File *>(pFile->fHandle);
  f->seek(iPosition);
  pFile->iPos = (int32_t)f->position();
  i = micros() - i;
//  Serial.printf("Seek time = %d us\n", i);
  return pFile->iPos;
 } /* GIFSeekFile() */

 unsigned long start_tick = 0;


void ShowGIF(char *name)
{
  start_tick = millis();
   
  if (gif.open(name, GIFOpenFile, GIFCloseFile, GIFReadFile, GIFSeekFile, GIFDraw))
  {
    x_offset = (MATRIX_WIDTH - gif.getCanvasWidth())/2;
    if (x_offset < 0) x_offset = 0;
    y_offset = (MATRIX_HEIGHT - gif.getCanvasHeight())/2;
    if (y_offset < 0) y_offset = 0;
    Serial.printf("Successfully opened GIF; Canvas size = %d x %d\n", gif.getCanvasWidth(), gif.getCanvasHeight());
    Serial.flush();
    while (gif.playFrame(true, NULL))
    {      
      if ( (millis() - start_tick) > 8000) { // we'll get bored after about 8 seconds of the same looping gif
        break;
      }
    }
    gif.close();
  }

} /* ShowGIF() */

//} // end loop


void rebootESP(String message) {
  Serial.print("Rebooting ESP32: "); Serial.println(message);
  ESP.restart();
}

 
String listFiles(bool ishtml) {  // list all of the files, if ishtml=true, return html rather than simple text
  String returnText = "";
  Serial.println("Listing files stored on SPIFFS");
  File root = SPIFFS.open("/");
  File foundfile = root.openNextFile();
  if (ishtml) {
    returnText += "<table><tr><th align='left'>Name</th><th align='left'>Size</th><th align='justify'>GIF</th><th></th><th></th></tr>";
  }
  while (foundfile) {
    if (ishtml) {
      returnText += "<tr align='left'><td>" + String(foundfile.name()) + "</td><td>" + humanReadableSize(foundfile.size()) + "</td>";
      returnText += "<td>""<img src=" "file?name=" + String(foundfile.name()) + "&action=show>";
      returnText += "<td><button onclick=\"downloadDeleteButton(\'" + String(foundfile.name()) + "\', \'play\')\">Play</button>";
      returnText += "<td><button onclick=\"downloadDeleteButton(\'" + String(foundfile.name()) + "\', \'download\')\">Download</button>";
      returnText += "<td><button onclick=\"downloadDeleteButton(\'" + String(foundfile.name()) + "\', \'delete\')\">Delete</button></tr>";
      
    } else {
      returnText += "File: " + String(foundfile.name()) + " Size: " + humanReadableSize(foundfile.size()) + "\n";
    }
    foundfile = root.openNextFile();
  }
  if (ishtml) {
    returnText += "</table>";
  }
  root.close();
  foundfile.close();
  return returnText;
  }

  
String humanReadableSize(const size_t bytes) {
  if (bytes < 1024) return String(bytes) + " B";
  else if (bytes < (1024 * 1024)) return String(bytes / 1024.0) + " KB";
  else if (bytes < (1024 * 1024 * 1024)) return String(bytes / 1024.0 / 1024.0) + " MB";
  else return String(bytes / 1024.0 / 1024.0 / 1024.0) + " GB";
}
 
/************************* Arduino Sketch Setup and Loop() *******************************/
void setup() {


  HUB75_I2S_CFG mxconfig(
  PANEL_RES_X*2,   // module width
  PANEL_RES_Y/2,   // module height
  NUM_ROWS*NUM_COLS 
  );

  mxconfig.gpio.a = A_PIN;
  mxconfig.gpio.b = B_PIN;
  mxconfig.gpio.c = C_PIN;
  
  mxconfig.clkphase = false;

    
  // Display Setup
  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->begin();
  dma_display->setBrightness8(sliderValue.toInt()); //0-255
  dma_display->clearScreen();

  // create OneEightMatrixDisplaylay object based on our newly created dma_display object
  OneEightMatrixDisplay = new VirtualMatrixPanel((*dma_display), NUM_ROWS, NUM_COLS, PANEL_RES_X, PANEL_RES_Y, SERPENT, TOPDOWN);

  // THE IMPORTANT BIT BELOW!
  OneEightMatrixDisplay->setPhysicalPanelScanRate(ONE_EIGHT_32);

  
  Serial.begin(115200);

  Serial.print("Firmware: "); Serial.println(FIRMWARE_VERSION);

  Serial.println("Booting ...");

  Serial.println("Mounting SPIFFS ...");
  if (!SPIFFS.begin(true)) {
    // if you have not used SPIFFS before on a ESP32, it will show this error.
    // after a reboot SPIFFS will be configured and will happily work.
    Serial.println("ERROR: Cannot mount SPIFFS, Rebooting");
    rebootESP("ERROR: Cannot mount SPIFFS, Rebooting");
  }

  Serial.begin(115200);
  Serial.println("Starting AnimatedGIFs Sketch");

  // Start filesystem
  Serial.println(" * Loading SPIFFS");
  if(!SPIFFS.begin()){
  Serial.println("SPIFFS Mount Failed");
  }
  

  Serial.print("SPIFFS Free: "); Serial.println(humanReadableSize((SPIFFS.totalBytes() - SPIFFS.usedBytes())));
  Serial.print("SPIFFS Used: "); Serial.println(humanReadableSize(SPIFFS.usedBytes()));
  Serial.print("SPIFFS Total: "); Serial.println(humanReadableSize(SPIFFS.totalBytes()));

  Serial.println(listFiles());

  Serial.println("Loading Configuration ...");

  config.ssid = default_ssid;
  config.wifipassword = default_wifipassword;
  config.httpuser = default_httpuser;
  config.httppassword = default_httppassword;
  config.webserverporthttp = default_webserverporthttp;

  Serial.print("\nConnecting to Wifi: ");
  WiFi.begin(config.ssid.c_str(), config.wifipassword.c_str());
  (WiFi.status() != WL_CONNECTED); 
  //{ delay(500);
  //Serial.print("."); //Uncomment the 3 lines if you want the ESP32 to wait until the WIFI is connected
  // }
 Serial.println("\n\nNetwork Configuration:");
  Serial.println("----------------------");
  Serial.print("         SSID: "); Serial.println(WiFi.SSID());
  Serial.print("  Wifi Status: "); Serial.println(WiFi.status());
  Serial.print("Wifi Strength: "); Serial.print(WiFi.RSSI()); Serial.println(" dBm");
  Serial.print("          MAC: "); Serial.println(WiFi.macAddress());
  Serial.print("           IP: "); Serial.println(WiFi.localIP());
  Serial.print("       Subnet: "); Serial.println(WiFi.subnetMask());
  Serial.print("      Gateway: "); Serial.println(WiFi.gatewayIP());
  Serial.print("        DNS 1: "); Serial.println(WiFi.dnsIP(0));
  Serial.print("        DNS 2: "); Serial.println(WiFi.dnsIP(1));
  Serial.print("        DNS 3: "); Serial.println(WiFi.dnsIP(2));
  Serial.println();

  // configure web server
  Serial.println("Configuring Webserver ...");
  server = new AsyncWebServer(config.webserverporthttp);
  configureWebServer();

  // startup web server
  Serial.println("Starting Webserver ...");
  server->begin();
  
  /* all other pixel drawing functions can only be called after .begin() */
  OneEightMatrixDisplay->fillScreen(dma_display->color565(0, 0, 0));
  dma_display->setTextSize(1);
  delay(1000);
  OneEightMatrixDisplay->print("IP:"); OneEightMatrixDisplay->print(WiFi.localIP());  
  delay(4000);
  OneEightMatrixDisplay->fillScreen(dma_display->color565(0, 0, 0));
  gif.begin(LITTLE_ENDIAN_PIXELS);

}

 String gifDir = "/"; // play all GIFs in this directory on the SD card
 char filePath[256] = { 0 };
 File root, gifFile;


void loop() 

{
       if (shouldReboot) {
    rebootESP("Web Admin Initiated Reboot");
  }
   while (1) // run forever
   {
            if (shouldReboot) {
    rebootESP("Web Admin Initiated Reboot");
  } 
      root = FILESYSTEM.open(gifDir);
      if (root)
      {
           gifFile = root.openNextFile();
            while (gifFile)
            {
              if (!gifFile.isDirectory()) // play it
              {
                
                // C-strings... urghh...                
                memset(filePath, 0x0, sizeof(filePath));                
                strcpy(filePath, gifFile.path());
                
                // Show it.
                ShowGIF(filePath);
               
              }
             // dma_display->fillScreen(dma_display->color565(0, 0, 0)); 
            //gifFile.close(); // uncomment these 3 lines to play all GIFS in SPIFF in a loop.
           //gifFile = root.openNextFile();
            }
         root.close();
      }  //root
      
      delay(1000); // pause before restarting  
   }
}
