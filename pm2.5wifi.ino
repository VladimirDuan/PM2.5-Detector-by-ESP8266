#include <U8g2lib.h>
#include <U8x8lib.h>
#include <Arduino.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include "WifiConfig.h"
#include "httpServer.h"
#include "gps.h"
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WString.h>
//#include <U8g2lib.h>


#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);
//U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 14, /* dc=*/ 12, /* reset=*/ 13);
httpServer server;
WifiConfig wifiConfig;
gps gpsClient;
uint16_t pm25;

uint16_t pm25temp = 0;

uint16_t pm10;
char mac[30];
String lat = "N/A";
String lon = "N/A";

void setup()
{
  u8g2.begin();
  u8g2.clearBuffer();          // clear the internal menory
  u8g2.setFont(u8g2_font_ncenB14_tr); // choose a suitable font
  u8g2.drawStr(0, 35, " Loading....."); // write something to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display

  Serial.begin(9600);

  wifiConfig.espNKWiFiconnect();
  pinMode(D2, INPUT);
  // show IP address
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  uint8_t mac8[WL_MAC_ADDR_LENGTH];
  WiFi.macAddress(mac8);
  sprintf(mac, "%02X:%02X:%02X:%02X:%02X:%02X", mac8[0], mac8[1], mac8[2], mac8[3], mac8[4], mac8[5]);

  //Serial.print("MAC address: ");
  //Serial.println(mac);

  // init basic web server for basic functions: info, restart, cleareeprom
  wifiConfig.initBasicHttpServer();
  server.login();
  while (Serial.read() >= 0);
}

void loop()
{
  wifiConfig.server.handleClient();
  if (wifiConfig.gpsFlag) {
    gpsUpdate();
  }
  uint8_t pmArray[10];
  serialDetect(pmArray);
  if ((pmArray[0] == 0xAA) && (pmArray[1] == 0xC0) && (pmArray[9] == 0xAB))
  {
    pm25 = pmArray[2] + (pmArray[3] << 8);
    pm10 = pmArray[4] + (pmArray[5] << 8);

    u8g2.clearBuffer();          // clear the internal menory
    u8g2.setFont( u8g2_font_7x14_mr ); // choose a suitable font
    char p[30];
    sprintf(p, "WiFi:%12s", WiFi.SSID().c_str());
    u8g2.drawStr(0, 13, p);

    u8g2.setFont( u8g2_font_7x14B_mr ); // choose a suitable font
    sprintf(p, "  PM 2.5: %4d.%d", pm25 / 10, pm25 % 10);
    u8g2.drawStr(0, 30, p);
    sprintf(p, "  PM  10: %4d.%d", pm10 / 10, pm10 % 10);
    u8g2.drawStr(0, 45, p);

    char json[120];
    sprintf(json, "{ \"MAC\": \"%s\", \"PM25\": \"%4d.%d\", \"PM10\": \"%4d.%d\", \"lat\": \"%s\", \"lon\": \"%s\" }", mac, pm25 / 10, pm25 % 10, pm10 / 10, pm10 % 10, lat.c_str() , lon.c_str() );

    //Serial.println(json);

    u8g2.setFont( u8g2_font_7x14_mr );

    if (WiFi.localIP()[0] != 0) {
      sprintf(p, "ip:%3d.%3d.%3d.%3d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
      u8g2.drawStr(0, 60, p);

      if (pm25temp - pm25 > 10 || pm25 - pm25temp > 10)
        server.postData(json);
      pm25temp = pm25;

    }
    else {
      sprintf(p, "WiFi Disconnected");
      u8g2.drawStr(0, 60, p);
    }
    u8g2.sendBuffer();
  }
}

void serialDetect(uint8_t * buff)
{
  if (Serial.available())
  {
    delay(100);
    int temp = 0;
    while (Serial.available())
    {
      if (temp <= 9) {
        *(buff + temp) = Serial.read();
        temp++;
      }
      else {
        temp = 0;
      }
    }
  }
  else
  {
    delay(100);
    serialDetect(buff);
  }
}

void gpsUpdate() {

  if (!gpsClient.gpsUpdated) {
    u8g2.setFont( u8g2_font_7x14_mr ); // choose a suitable font
    char p[30];
    sprintf(p, " GPS Detecting...   ");
    u8g2.drawStr(0, 13, p);
    u8g2.sendBuffer();
    gpsClient.gpsRead();
  }
  else {
    wifiConfig.gpsFlag = false;
    gpsClient.gpsUpdated = false;
    lat = gpsClient.latitude;
    lon = gpsClient.longitude;
  }

}

