//Created by: Mohan Palanisamy
//Assumes the MQTT broker has a REST capable HTTP End point similar to Ponte
//This code was tested using a Eclipse Ponte bridge and the
//Arduino Yun Http endpoints from the Bridge Library
//PUT has been tested. POST seems to work reasonably well. GET has kinks in it.

#include <SoftwareSerial.h>
#include <Arduino.h>

class ESP8266RESTHelper
{
  public:
      ESP8266RESTHelper(void);
      
      boolean begin(void);
      boolean begin(String WifiNetworkNameSSID, String WifiPassword);
      boolean sendMQTTMessage(String server, int port,String topic,String message);
      
      //These methods are work in progress. Do not use. Not tested.
      boolean httpPOST(String server, int port,String resourceURI,String content,String* response);
      
      boolean httpPUT(String server, int port,String resourceURI,String content,String* response);
      
        //These methods are work in progress. Do not use. Not tested.
      boolean httpGET(String server, int port,String resourceURI, String* response);

  private:
      boolean httpAction(String httpVerb, String server, int port,String resourceURI,String content, String* response);
};
