#ifndef gps_H
#define gps_H
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>
class gps {
  public:
    gps();
    String latitude;    //La
    String longitude;   //Lo
    bool gpsUpdated;
    void gpsRead();
  private:
    SoftwareSerial *gpsSerial;
    char nmeaSentence[68];
    String parseGprmcLat(String s);
    String parseGprmcLon(String s);
    void gpsReadEEPROM();
    void gpsSaveEEPROM();
};

#endif
