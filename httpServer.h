#ifndef httpServer_H
#define httpServer_H

#include <WiFiClient.h>
#include <ESP8266WiFi.h>

const char host[50] = "10.3.8.211";

const int httpPort = 80;
static String link = "/";
static String serverLink = "/scenic/CameraDataServlet.do";
static String serverHost = "www.bjfast.com";
static const char* serverHostC = "www.bjfast.com";


const String DDDDD = "2014290002";  // your account (Student ID)

const String PW = "555718";  // your password (For network login)

const String httpPostList =       "POST " + link + " HTTP/1.1\r\n"
                                  "Host: " + host + ":" + httpPort + "\r\n"
                                  "User-Agent: PM2.5 Detector/1.0\r\n"
                                  "Connection: close\r\n"
                                  "Content-Type: application/x-www-form-urlencoded\r\n";

static String httpPostJson =             "POST " + serverLink + " HTTP/1.1\r\n"
    "Host: " + serverHost + ":" + httpPort + "\r\n"
    "User-Agent: PM2.5 Detector/1.0\r\n"
    "Connection: close\r\n"
    "Content-Type: application/json\r\n";


class httpServer {
  public:
    httpServer();
    WiFiClient httpClient;
    void postData(char* data);
    void postList();
    void login();
  private:
    void serverConnect();
    void serverDisconnect();
    void postJson(int len, String params);
    void post(int len, String params);
    void readBuffer();
    void handleConnectionFailed();
};
#endif
