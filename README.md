# ESP32-GPS
Connect GPS Modules to ESP32 to read all data output and send PGKC commands to the GPS.

![IMG_20230526_201422](https://github.com/OldMate6288/ESP32-GPS/assets/93004427/4c8c65c4-607f-4fed-b6c3-9a64cb975a39)


![192 168 4 1 - Google Chrome 26_05_2023 8_16_29 PM_LI](https://github.com/OldMate6288/ESP32-GPS/assets/93004427/02642071-29f9-414b-b561-8150f53eaeb4)


Tested on an EBYTE GPS module but it should theoretically work with any GPS module with TinyGPS++ support.

Also the LCD is a 0.96" 80x160 ST7735 display

EBYTE GPS MODULE: https://www.cdebyte.com/products/E108-GN02D/2#Pin

# HOW TO SETUP

## 1. Connect everything to your ESP32 as follows:

### LCD PINS:

MISO -> 19

MOSI -> 23

SCLK -> 18

CS   -> 5

DC   -> 2

RST  -> 4

### GPS MODULE:

GPS TX -> RX 16

GPS RX -> TX 17

## 2. Install the following Arduino libraries either from GitHub or directly in Arduino IDE:

TFT_eSPI

TinyGPS++

ESPAsyncWebServer

AsyncTCP

## 3. Edit WiFi.softAP in the code to your desired Access Point name and password

EXAMPLE:
WiFi.softAP("YOURAPNAME", "YOURAPPASSWORD");

## 4. Run the code, then connect to the WiFi access point and naviagte to 192.168.4.1 on your device.

## 5. Wait for your GPS to get satellite signal and you should start to see your coordinates and other data displayed in real time on the LCD and webserver.

# ISSUES

1. I haven't been able to find out a way (yet) to upload Rinex GPS data files to the GPS module from the ESP32. So currently there is no way to Hot Start the GPS, meaning it will Cold Start on every boot and can take upwards of 20 minutes to retrieve satellite signal unless your GPS has a serial converter and some sort of software provided to upload the file.

2. The LCD screen will also flicker every second because I haven't used proper screen buffering techniques (only tft.fillScreen() is used). I may fix this in the future as it's easy to do so, but it's also time consuming adding all the individual characters and isn't affecting performance so I'll focus on it after I solve the data file issue.



### ENJOY!

P.S. ANY CONTRIBUTIONS TO THE CODE ARE HIGHLY APPRECIATED :)
