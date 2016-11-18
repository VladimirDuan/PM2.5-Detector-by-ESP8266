#include "httpServer.h"

httpServer::httpServer() {
}


void httpServer::serverConnect() {
  if (!httpClient.connect(serverHostC, httpPort)) {
    Serial.println("Connection failed\n");
    handleConnectionFailed();
  }
  else {
    Serial.println("Connected to the server\n");
  }
}

void httpServer::login() {
  if (!httpClient.connect("10.3.8.211", httpPort)) {
    Serial.println("Campus network connection failed\n");
    return;
  }
  else {
    Serial.println("Connected to the campus network server\n");
  }
  postList();
  delay(1000);
  //readBuffer();
}

void httpServer::serverDisconnect() {
  httpClient.stop();
  Serial.println("Server disconnected\n");
}

void httpServer::postList() //http POST
{
  int allDataLength = ("DDDDD=" + DDDDD + "&upass=" + PW + "&R1=0").length();
  post(allDataLength, "DDDDD=" + DDDDD + "&upass=" + PW + "&R1=0");
}

void httpServer::post(int len, String params)
{
  httpClient.print(httpPostList);
  httpClient.print("Content-Length: ");
  httpClient.print(len);
  httpClient.print("\r\n");
  httpClient.print("\r\n");
  httpClient.print(params);
  httpClient.print("\n");
  delay(10);
}

void httpServer::postData(char* data) //http POST
{
  serverConnect();
  int dataLength = String(data).length();
  postJson(dataLength, data);
  Serial.println("One pack sent.\n");
  readBuffer();
  serverDisconnect();
}

void httpServer::postJson(int len, String params)
{
  httpClient.print(httpPostJson);
  httpClient.print("Content-Length: ");
  httpClient.print(len);
  httpClient.print("\r\n");
  httpClient.print("\r\n");
  httpClient.print(params);
  httpClient.print("\n");
  delay(10);
}

void httpServer::readBuffer() {
  Serial.println("----- Reply from the server is  -----");
  while (httpClient.available()) {
    String line = httpClient.readStringUntil('\r');
    Serial.println(line);
  }
  Serial.println("----- Reply from the server end -----");
}

void httpServer::handleConnectionFailed() {

}
