#define ACCINDEX 0

#include "WIFI_manager.h"
#include "MQTT_manager.h" 

#include <NTPClient.h>

const double sendRate = 0.5; //Hz

WiFiEspUDP Udp;
unsigned long lastSync = 0;
volatile unsigned long epoch = 0;
NTPClient timeClient(Udp);

void setup() {
  pinMode(13,OUTPUT);
  digitalWrite(13, HIGH);
  // initialize serial for debugging
  Serial.begin(115200);

  WIFI::init();
  MQTT::init();

  WIFI::wait();  

  timeClient.begin();
  while(!timeClient.update()){
      delay(1000);
  }
  Serial.println("requesting unix time");
  Serial.println(epoch = timeClient.getEpochTime());
  lastSync = millis();
  Serial.println();
  timeClient.end();

  digitalWrite(13, LOW); 
  
}

void loop() {
  unsigned long elapsed = millis();
  if(!MQTT::connected()){
    WIFI::wait();
    MQTT::connect();
  }
  MQTT::sendMeasures(epoch, lastSync);
  int d = (1000 / sendRate) - (millis() - elapsed);
  if(d <= 0)
    delay(0);
  else
    delay(d);
  MQTT::loop();
}

