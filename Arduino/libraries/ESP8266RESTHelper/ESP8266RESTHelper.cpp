//Created by: Mohan Palanisamy
//1. Assumes the MQTT broker has a REST capable HTTP End point similar to Ponte
//This code was tested using a Eclipse Ponte bridge and the
//Arduino Yun Http endpoints from the Bridge Library
//2. PUT has been tested. POST seems to work reasonably well. GET has kinks in it.
//3. Please note there is a Timeout value set here that will determine how long the 
//software serial will wait for a response. Take that in to account in any delay() you 
//program in to your sketch.
//4. Uncomment the _DEBUG flag if you want to see Serial.println()s sent to the Serial Port
//   If you do so, you might want to comment the Serial.begin() in your sketch becuase this opens the serial

#include <SoftwareSerial.h>
#include "ESP8266RESTHelper.h"

//#define _DEBUG

//using Digital 2 and 3 pins for connecting to ESP as software serial
#define ESP_RX_PIN 2
#define ESP_TX_PIN 3
#define TIMEOUT 1000

SoftwareSerial esp8266Serial(ESP_RX_PIN, ESP_TX_PIN);
boolean isESP8266Available = false;

ESP8266RESTHelper::ESP8266RESTHelper()
{
}

//begins a ESP Session. Assumes that Wireless configuration has already been done and connected to network.
//IP address is already set
boolean ESP8266RESTHelper::begin(void)
{

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

  if (esp8266Serial.find("OK"))
  {
    isESP8266Available = true;
  }

  #if defined(_DEBUG)
  if (isESP8266Available)
  {
    Serial.println("ESP 8266 is available and responding..");
   
  }
  else
    Serial.println("ESP 8266 is not available..");
  #endif
  
  return isESP8266Available;
}

boolean ESP8266RESTHelper::begin(String WifiNetworkNameSSID, String WifiPassword)
{
 
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

  if (esp8266Serial.find("OK"))
  {
    isESP8266Available = true;
  }

  #if defined(_DEBUG)
  if (isESP8266Available)
  {
    Serial.println("ESP 8266 is available and responding..");
    Serial.println("Connecting to WiFi Network: " + WifiNetworkNameSSID);
  }
  else
    Serial.println("ESP 8266 is not available..");
  #endif
  
  String wifiConnectCommand="AT+CWJAP=\"" + WifiNetworkNameSSID + "\",\"" + WifiPassword + "\"";
  Serial.println(wifiConnectCommand);
  esp8266Serial.println(wifiConnectCommand);
  
  if (esp8266Serial.find("OK"))
  {
    isESP8266Available = true;
    
    Serial.println("Connected to WiFi Network: " + WifiNetworkNameSSID);
  }else
  {
    if (esp8266Serial.find("FAIL"))
       Serial.println("Unable to connect to WiFi Network: " + WifiNetworkNameSSID);
    isESP8266Available=false;
  }
  
   return isESP8266Available;
}

boolean ESP8266RESTHelper::sendMQTTMessage(String server, int port, String topic, String message)
{
  String response = "";
  return httpAction("PUT", server, port, topic, message, &response);
}

//These methods are work in progress. Not tested fully. Use at your own risk and modify as you see fit.
boolean ESP8266RESTHelper::httpPOST(String server, int port, String resourceURI, String content, String* response)
{
  return httpAction("POST", server, port, resourceURI, content, response );
}
//These methods are work in progress. Not tested fully. Use at your own risk and modify as you see fit.
boolean ESP8266RESTHelper::httpPUT(String server, int port, String resourceURI, String content, String* response)
{
  return httpAction("PUT", server, port, resourceURI, content, response );
}
//These methods are work in progress. Not tested fully. Use at your own risk and modify as you see fit.
boolean ESP8266RESTHelper::httpGET(String server, int port, String resourceURI, String* response)
{
  return httpAction("GET", server, port, resourceURI, "", response );
}


boolean ESP8266RESTHelper::httpAction(String httpVerb, String server, int port, String resourceURI, String content, String* response)
{
  if (!isESP8266Available) return false;

  //Prepare all command strings

  //1. prepare TCP Connection AT Command
  String startTCPSession = "AT+CIPSTART=\"TCP\",\"" + server + "\"," + port;

  //2. prepare PUT Request AT Command
  String tcpPacket =
    httpVerb + " " + resourceURI + " HTTP/1.1\r\n"
    "Host: " + server + ":" + port + "\r\n"
    "Accept: */*\r\n";

  if (!httpVerb.equals("GET"))
  {
    tcpPacket = tcpPacket +
                   "Content-Length: " + content.length() + "\r\n"
                   "Content-Type: application/x-www-form-urlencoded\r\n"
                   "\r\n" +
                   content;
  }
  
 // Serial.println(putTCPPacket);
  //3. prepare Length of TCP Packet ATP Command
  String temp_ATCommand = "AT+CIPSEND=";
  String lengthTCPPacket = temp_ATCommand + tcpPacket.length();


  //AT Commands are ready.. Lets Send them after checking at each command.
  //Each check is a blocking call.

#if defined(_DEBUG)
  Serial.println("Opening TCP Connection..");
  Serial.println("Command : " + startTCPSession);
#endif

  esp8266Serial.println(startTCPSession);
 
  if (esp8266Serial.find("Linked"))
  {
#if defined(_DEBUG)
    Serial.println("Connected..");
    Serial.println("Sending Packet Length...");
    Serial.println("Command : " + lengthTCPPacket);
#endif

    esp8266Serial.println(lengthTCPPacket);

#if defined(_DEBUG)
    Serial.println("Sending Packet....");
    Serial.println(tcpPacket);
#endif

    esp8266Serial.println(tcpPacket);
 
    if (esp8266Serial.find("SEND OK"))
    {
#if defined(_DEBUG)
      Serial.println("Packet Sent!!!");
#endif
        while (esp8266Serial.available())
        {
          String esp8266SerialResponse = esp8266Serial.readString();
          
          response = &esp8266SerialResponse;
        }
   
      //After reading close the connection. Do not want to reuse because the TCP stack on
      //ESP8266 might close the connection after a while.
      //So make sure the connection is closed by sending AT+CIPCLOSE
      esp8266Serial.println("AT+CIPCLOSE");
      return true;
    } else
    {
#if defined(_DEBUG)
      Serial.println("Sending Packet Failed");
#endif
      return false;
    }
  } else
  {
#if defined(_DEBUG)
    Serial.println("Cannot Start TCP Session");
#endif

    return false;
  }
}


