//Created by: Mohan Palanisamy
//This code was tested using a Eclipse Ponte bridge and the
//Arduino Yun Http endpoints from the Bridge Library
 

#include <SoftwareSerial.h>
#include "ESP8266RESTHelper.h"

ESP8266RESTHelper esp8266=ESP8266RESTHelper();

void setup() {
 
  //if(esp8266.begin("your wifi network SID","wifi password")))
  //    connected...
  //else
  //    not connected.
  Serial.begin(9600);
  //I am doing just a begin because my ESP8266 has the IP address set already..
   esp8266.begin();
  
    String response="";
    if(esp8266.httpPOST("192.168.1.205",80, "/data/put/FromArduinoESP3/Hello5", "",&response))
    {
       Serial.println("HTTP POST to Arduino Data Store successful");
       
    }else
    {
     Serial.println("HTTP POST to Arduino Data Store  Failed");
    }
// if(esp8266.httpPOST("192.168.1.205",80, "/mailbox/", "Hello From ESP",&response))
// {
//     Serial.println("HTTP POST to Yun Messagebox successful");
//      Serial.println(response);
// }else
// {
//     Serial.println("HTTP POST to Yun Messagebox Failed");
//      Serial.println(response);
// }
 
// if(esp8266.httpGET("192.168.1.205",80, "/data/get/test",&response))
// {
//     Serial.println("HTTP GET successful");
//      Serial.println(response);
// }else
// {
//     Serial.println("HTTP GET Failed");
//      Serial.println(response);
// }
 
 
}

void loop() {
  

}
