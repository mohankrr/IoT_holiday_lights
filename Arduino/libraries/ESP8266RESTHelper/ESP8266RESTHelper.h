//Created by: Mohan Palanisamy
//Assumes the MQTT broker has a REST capable HTTP End point similar to Ponte
//This code was tested using a Eclipse Ponte Broker and the
//Arduino Yun Http endpoints from the Bridge Library
#include <SoftwareSerial.h>
#include <Arduino.h>

class ESP8266RESTHelper
{
  public:
      ESP8266RESTHelper(void);
      
      boolean begin(void);
       
      boolean sendMQTTMessage(String server, int port,String topic,String message);
      
      //These methods are work in progress. Do not use. Not tested.
      boolean httpPOST(String server, int port,String resourceURI,String content);
      
      boolean httpPUT(String server, int port,String resourceURI,String content);
      
        //These methods are work in progress. Do not use. Not tested.
      String httpGET(String server, int port,String resourceURI);

  private:
      boolean httpAction(String httpVerb, String server, int port,String resourceURI,String content);
};
