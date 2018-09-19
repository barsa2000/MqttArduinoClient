#define ACCINDEX 1

#include "DEBUG.h"
#include "WIFI_manager.h"
#include "MQTT_manager.h" 
#include "WiFiEspUdp.h"    //WiFiEspUDP
#include <NTPClient.h>     //NTPClient


// frequenza di registrazione e invio dei dati
const double sendRate = 0.5;

// variabili per la ricezione del tempo in formato Unix Time
WiFiEspUDP Udp;
unsigned long lastSync = 0;
volatile unsigned long epoch = 0;
NTPClient timeClient(Udp);

void setup() {
  // inizializzazione e accensione del led su scheda Arduino
  pinMode(13,OUTPUT);
  digitalWrite(13, HIGH);

  // inizializzazione porta seriale per il debug
  Serial.begin(115200);

  // inizializzazione connessione WiFi
  WIFI::init();
  // inizializzazione client MQTT
  MQTT::init();
  // attesa della riuscita connessione alla rete wifi
  WIFI::wait();  

  // inizializzazione connessione al server NTP
  timeClient.begin();
  // attesa ricezione da server NTP
  while(!timeClient.update()){
      delay(1000);
  }
  // stampe su seriale di debug
  DEBUG_PRINTLN("requesting unix time");
  epoch = timeClient.getEpochTime()
  DEBUG_PRINTLN(epoch);
  // aggiornamento momento dell'ultima sincronizzazione
  lastSync = millis();
  // chiusura della connessione al server NTP
  timeClient.end();

  // spegnimento led su scheda Arduino
  digitalWrite(13, LOW); 
}

void loop() {
  // salvataggio tempo passato in millisecondi dall'inizio dell'esecuzione
  unsigned long elapsed = millis();
  // controllo dello stato della connessione del client MQTT
  if(!MQTT::connected()){
    // attesa della riuscita connessione alla rete wifi
    WIFI::wait();
    // riconnessione al server MQTT
    MQTT::connect();
  }
  // invio dei dati
  MQTT::sendMeasures(epoch, lastSync);
  // calcolo del tempo rimanente da attendere per l'esecuzione di un ciclo
  //int d = (1000 / sendRate) - (millis() - elapsed);
  // attendo 0 secondi se il tempo rimanente è negativo altrimenti attendo il
  // tempo calcolato
  /*
  if(d < 0)
    delay(0);
  else
    delay(d);
    */
  // esegui il loop del client MQTT
  MQTT::loop();
}

