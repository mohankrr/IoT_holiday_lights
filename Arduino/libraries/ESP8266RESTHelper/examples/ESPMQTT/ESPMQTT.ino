#include <SoftwareSerial.h>
#include "ESP8266RESTHelper.h"

ESP8266RESTHelper esp8266=ESP8266RESTHelper();

void setup() {
  
  esp8266.begin();

}

void loop() {
  //Send Red
  esp8266.sendMQTTMessage("192.168.1.217",6000,"/resources/FromArduinoUnoESP/RGB","255,0,0");
  delay(2000);

  //send Green
  esp8266.sendMQTTMessage("192.168.1.217",6000,"/resources/FromArduinoUnoESP/RGB","0,255,0");
  delay(2000);
  
  //Send blue
  esp8266.sendMQTTMessage("192.168.1.217",6000,"/resources/FromArduinoUnoESP/RGB","0,0,255");
  delay(2000);

}

