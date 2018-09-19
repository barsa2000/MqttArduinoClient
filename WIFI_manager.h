#ifndef WIFI_manager_H
#define WIFI_manager_H

#include "WiFiEspClient.h"
#include "WiFiEsp.h"
#include "WiFiEspUdp.h"
#include "SoftwareSerial.h"

namespace WIFI{

namespace{
WiFiEspClient espClient;
SoftwareSerial soft(2, 3); // RX, TX
}

WiFiEspClient getClient() {
    return espClient;
}

void init() {
    soft.begin(9600);
    WiFi.init(&soft);
    WiFi.begin("SCIADROtest", "m05c4rd0");
}

void wait() {
  Serial.print("checking wifi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println();
}


}
#endif