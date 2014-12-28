//Created by: Mohan Palanisamy
//Assumes the MQTT broker has a REST capable HTTP End point similar to Ponte
//This code was tested using a Eclipse Ponte bridge and the
//Arduino Yun Http endpoints from the Bridge Library
#include <SoftwareSerial.h>
#include "ESP8266RESTHelper.h"

#define _DEBUG 

//using Digital 2 and 3 pins for connecting to ESP as software serial
#define ESP_RX_PIN 2     
#define ESP_TX_PIN 3
#define TIMEOUT 3000

SoftwareSerial esp8266Serial(ESP_RX_PIN,ESP_TX_PIN);
boolean isESP8266Available=false;

ESP8266RESTHelper::ESP8266RESTHelper()
{
}

//begins a ESP Session. Assumes that Wireless configuration has already been done and connected to network.
//IP address is already set 
boolean ESP8266RESTHelper::begin(void)
{
  Serial.println("Beginning the esp8266");
 // #ifdef _DEBUG
  //if(!Serial)  //Start serial for debugging.
  #if defined(_DEBUG)
    Serial.begin(9600);
    Serial.println("Debug Serial Port opened...");
  #endif
  
  esp8266Serial.begin(9600);
  esp8266Serial.setTimeout(TIMEOUT);
  
  esp8266Serial.println("AT");

 #if defined(_DEBUG)
  Serial.println("checking..");
#endif

  if(esp8266Serial.find("OK"))
  {
      isESP8266Available=true;
  }
  
   #if defined(_DEBUG)
  if(isESP8266Available)
    Serial.println("ESP 8266 is available and responding..");
  else
     Serial.println("ESP 8266 is not available..");
  #endif 
}

boolean ESP8266RESTHelper::sendMQTTMessage(String server, int port,String topic,String message)
{
    String resourceURI=topic;
    if(! (resourceURI.startsWith("/resources/") || resourceURI.startsWith("resources/")))
    {
       resourceURI="/resources/" + topic;
    }
    
    return httpAction("PUT", server, port, resourceURI, message);
}

//These methods are work in progress. Do not use. Not tested.
 boolean ESP8266RESTHelper::httpPOST(String server, int port,String resourceURI,String content)
 {
    return httpAction("POST", server, port, resourceURI, content );
 }
 
 boolean ESP8266RESTHelper::httpAction(String httpVerb, String server, int port,String resourceURI,String content)
 {
   if(!isESP8266Available) return false;
   
   //Prepare all command strings
   
   //1. prepare TCP Connection AT Command
   String startTCPSession= "AT+CIPSTART=\"TCP\",\"" + server +"\"," + port;

   //2. prepare PUT Request AT Command
   String putTCPPacket=
            httpVerb + " " + resourceURI + " HTTP/1.1\r\n"
            "Host: " + server + ":" + port + "\r\n"
            "Accept: */*\r\n"
            "Content-Length: " + content.length() + "\r\n"
            "Content-Type: application/x-www-form-urlencoded\r\n"
            "\r\n" +
             content;
    
   //3. prepare Length of TCP Packet ATP Command
   String temp_ATCommand="AT+CIPSEND=";
   String lengthTCPPacket=temp_ATCommand + putTCPPacket.length();
   

   //AT Commands are ready.. Lets Send them after checking at each command.
   //Each check is a blocking call. 
  
   #if defined(_DEBUG)
    Serial.println("Opening TCP Connection..");
    Serial.println("Command : " + startTCPSession);
    #endif
    
    esp8266Serial.println(startTCPSession);
   
    if(esp8266Serial.find("Linked"))
    {
       #if defined(_DEBUG)
       Serial.println("Connected.."); 
       Serial.println("Sending Packet Length...");
       Serial.println("Command : " + lengthTCPPacket);
       #endif
       
        esp8266Serial.println(lengthTCPPacket);

        #if defined(_DEBUG)
        Serial.println("Sending Packet....");
        Serial.println(putTCPPacket);
        #endif
          
        esp8266Serial.println(putTCPPacket);
        
        if(esp8266Serial.find("SEND OK"))
        {
             #if defined(_DEBUG)
           Serial.println("Packet Sent!!!");
           #endif
           //Not handling the output now.. It will be HTTP Response with no content.
           while(esp8266Serial.available())
           { 
               esp8266Serial.read();
            }
            
            //After reading close the connection. Do not want to reuse because the TCP stack on 
            //ESP8266 might close the connection after a while.
            //So make sure the connection is closed by sending AT+CIPCLOSE
            esp8266Serial.println("AT+CIPCLOSE");
           return true;
        }else
        {
            #if defined(_DEBUG)
          Serial.println("Sending Packet Failed");
          #endif
          return false;
        }
    }else
    {
        #if defined(_DEBUG)
        Serial.println("Cannot Start TCP Session");
        #endif
        
        return false;
    }
 }
 
 
