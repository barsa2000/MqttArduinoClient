#ifndef MQTT_manager_H
#define MQTT_manager_H

#include <PubSubClient.h>     //PubSubClient
#include <Adafruit_ADS1015.h> //Adafruit_ADS1115
#include <ArduinoJson.h>      //StaticJsonBuffer, JsonObject

namespace MQTT {

namespace {
// variabile per convertitore analogico-digitale
Adafruit_ADS1115 ads;
// variabile per il client MQTT
PubSubClient client(WIFI::espClient);

// variabili per la formazione di una struttura JSON
StaticJsonBuffer<200> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();
}

// funzione per l'avanzamento del ciclo MQTT
void loop() {
    client.loop();
}

//funzione per l'inizializzazione del convertitiore analogico-digitale e 
// configurazione dell'indirizzo ip e porta per comunicare con il broker MQTT
void init() {
    ads.begin();
    client.setServer("192.168.2.178", 1883);
}

// connessione al broker MQTT
void connect() {
    if(!client.connected()) {
        // crezione ID del client MQTT
        String id = String("Accelerometro") + ACCINDEX;
        // connessione al broker MQTT con l'ID precedentemente creato
        while (!client.connect(id.c_str())) {
            delay(1500);
        }
    }
}

// funzione per il controllo dello stato della connessione del client al broker
bool connected() {
    return client.connected();
}

// funzione per l'invio dei dati al broker
void sendMeasures(unsigned long epoch, unsigned long lastSync){
    // raccolta dati dei 3 assi da convertitiore analogico-digitale a 
    // struttura JSON
    root["x"] = ads.readADC_SingleEnded(0);
    root["y"] = ads.readADC_SingleEnded(1);
    root["z"] = ads.readADC_SingleEnded(2);
    // aggiunta a struttura JSON delle informazioni riguardanti al momento della 
    // cattura dei dati
    root["t0"] = epoch;
    root["t1"] = millis() - lastSync;

    // conversione da struttura JSON a stringa per l'invio dei dati attraverso
    // il client MQTT
    String output;
    root.printTo(output);
    String topic = String("Accelerometri/")+ACCINDEX;
    client.publish( topic.c_str(), output.c_str() );
    Serial.println(output.c_str());
}



}
#endif //MQTT_manager_H