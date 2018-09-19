#ifndef MQTT_manager_H
#define MQTT_manager_H

#include <PubSubClient.h>
#include <Adafruit_ADS1015.h>
#include <ArduinoJson.h>

namespace MQTT {

namespace {
Adafruit_ADS1115 ads;
PubSubClient client(WIFI::espClient);

StaticJsonBuffer<200> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();

void messageReceived(String &topic, String &payload) {
    Serial.println("incoming: " + topic + " - " + payload);
}

}

void loop() {
    client.loop();
}

void init() {
    ads.begin();
    client.setServer("192.168.2.178", 1883);
    //client.onMessage(messageReceived);
}

void connect() {
    if(!client.connected()) {
        while (!client.connect(String(String("Accelerometro")+ACCINDEX).c_str())) {
            delay(1500);
        }
    }
}

bool connected() {
    return client.connected();
}

void sendMeasures(unsigned long epoch, unsigned long lastSync){
    root["x"] = ads.readADC_SingleEnded(0);
    root["y"] = ads.readADC_SingleEnded(1);
    root["z"] = ads.readADC_SingleEnded(2);
    root["t0"] = epoch;
    root["t1"] = millis() - lastSync;

    String output;
    root.printTo(output);
    client.publish( String(String("Accelerometri/")+ACCINDEX).c_str(), output.c_str() );
    //root.prettyPrintTo(Serial);
    Serial.println(output.c_str());

}



}
#endif