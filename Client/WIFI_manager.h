#ifndef WIFI_manager_H
#define WIFI_manager_H

#include "DEBUG.h"
#include "WiFiEsp.h"        //WiFi
#include "WiFiEspClient.h"  //WiFiEspClient
#include "SoftwareSerial.h" //SoftwareSerial

namespace WIFI{

namespace{
// client della connessione WiFi
WiFiEspClient espClient;
// variabile per la replicazione di una porta seriale attraverso pin digitali
// per la connessione del chip ESP8266
SoftwareSerial soft(2, 3); // RX, TX
}
// funzione per l'inizializzazione della connessione all'ESP8266 e connessione
// alla rete WiFi
void init() {
    soft.begin(9600);
    WiFi.init(&soft);
    WiFi.begin("SCIADROtest", "m05c4rd0");
}

//funzione per l'attesa della connessione alla rete WiFi
void wait() {
  DEBUG_PRINT("checking wifi");
  while (WiFi.status() != WL_CONNECTED) {
    DEBUG_PRINT(".");
    delay(1000);
  }
  DEBUG_PRINTLN();
}

}
#endif //WIFI_manager_H