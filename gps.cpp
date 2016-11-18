#include "gps.h"

gps::gps() {
  gpsSerial = new SoftwareSerial(13, 12);
  gpsSerial->begin(9600);
  gpsUpdated = false;
  gpsReadEEPROM();
}

void gps::gpsRead() {
  while (gpsSerial->available());
  for (unsigned long start = millis(); millis() - start < 1000;) {
    while (gpsSerial->available()) {
      char c = gpsSerial->read();
      switch (c) {
        case '$':
          gpsSerial->readBytesUntil('*', nmeaSentence, 67);
          //Serial.println(nmeaSentence);
          latitude = parseGprmcLat(nmeaSentence);
          longitude = parseGprmcLon(nmeaSentence);
          gpsUpdated = true;
          gpsSaveEEPROM();
      }
    }
  }
}


String gps::parseGprmcLon(String s)
{
  int pLoc = 0; //paramater location pointer
  int lEndLoc = 0; //lat parameter end location
  int dEndLoc = 0; //direction parameter end location
  String lon;

  /*make sure that we are parsing the GPRMC string.
    Found that setting s.substring(0,5) == "GPRMC" caused a FALSE.
    There seemed to be a 0x0D and 0x00 character at the end. */
  if (s.substring(0, 4) == "GPRM")
  {
    //Serial.println(s);
    for (int i = 0; i < 7; i++)
    {
      if (i < 5)
      {
        pLoc = s.indexOf(',', pLoc + 1);
        /*Serial.print("i < 3, pLoc: ");
          Serial.print(pLoc);
          Serial.print(", ");
          Serial.println(i);*/
      }
      if (i == 5)
      {
        lEndLoc = s.indexOf(',', pLoc + 1);
        lon = s.substring(pLoc + 1, lEndLoc);
        /*Serial.print("i = 3, pLoc: ");
          Serial.println(pLoc);
          Serial.print("lEndLoc: ");
          Serial.println(lEndLoc);*/
      }
      else
      {
        dEndLoc = s.indexOf(',', lEndLoc + 1);
        lon = lon + " " + s.substring(lEndLoc + 1, dEndLoc);
        /*Serial.print("i = 4, lEndLoc: ");
          Serial.println(lEndLoc);
          Serial.print("dEndLoc: ");
          Serial.println(dEndLoc);*/
      }
    }
    return lon;
  }
}


String gps::parseGprmcLat(String s)
{
  int pLoc = 0; //paramater location pointer
  int lEndLoc = 0; //lat parameter end location
  int dEndLoc = 0; //direction parameter end location
  String lat;
  /*make sure that we are parsing the GPRMC string.
    Found that setting s.substring(0,5) == "GPRMC" caused a FALSE.
    There seemed to be a 0x0D and 0x00 character at the end. */
  if (s.substring(0, 4) == "GPRM")
  {
    //Serial.println(s);
    for (int i = 0; i < 5; i++)
    {
      if (i < 3)
      {
        pLoc = s.indexOf(',', pLoc + 1);
        /*Serial.print("i < 3, pLoc: ");
          Serial.print(pLoc);
          Serial.print(", ");
          Serial.println(i);*/
      }
      if (i == 3)
      {
        lEndLoc = s.indexOf(',', pLoc + 1);
        lat = s.substring(pLoc + 1, lEndLoc);
        /*Serial.print("i = 3, pLoc: ");
          Serial.println(pLoc);
          Serial.print("lEndLoc: ");
          Serial.println(lEndLoc);*/
      }
      else
      {
        dEndLoc = s.indexOf(',', lEndLoc + 1);
        lat = lat + " " + s.substring(lEndLoc + 1, dEndLoc);
        /*Serial.print("i = 4, lEndLoc: ");
          Serial.println(lEndLoc);
          Serial.print("dEndLoc: ");
          Serial.println(dEndLoc);*/
      }
    }
    return lat;
  }
}

void gps::gpsReadEEPROM() {
  latitude = "N/A";
  longitude = "N/A";
}

void gps::gpsSaveEEPROM() {

}
