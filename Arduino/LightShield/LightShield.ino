//Description: Arduino Yun sketch to read an MQTT Topic on Ponte bridge using HTTP Client
//Code uses a custom version of Infineon RGB shield library created with samples from Infineon
//Written for the Internet of Holiday Lights challange at element14.com
//http://www.element14.com/community/groups/arduino/blog/2014/12/11/intro-interactive-wifi-christmas-tree
//Date: December 26, 2014
//Author: Mohan Palanisamy
//Please note that the code doesn't include much of error handling as its a Proof of Concept code.


#include <Bridge.h>
#include <HttpClient.h>
#include <Wire.h>
#include "InfineonRGB.h"
  
//Using two strings to easily differentiate base url and the topic..
//Base url points to my local installation of Ponte server. /resources is where ponte adds topics.
String mqttPonteBaseUrl="http://192.168.1.217:6000/resources/";
String mqttTopic="FromPonte/RGB";
String currentColor="";

HttpClient httpClient;
InfineonRGB rgbShield= InfineonRGB();
   
void setup() {
  Bridge.begin();
  Wire.begin();
  rgbShield.begin();
  delay(1000); //small delay to let bridge initialize
}

void loop() {
  
  httpClient.get(mqttPonteBaseUrl+mqttTopic);

  while (httpClient.available()) 
  {
    String response=httpClient.readString();
    //Ponte bridge responds with a Not Found if there is no message for the topic.
    if(!response.equals("Not found"))
    {
       changeColor(response);
    }
  }
 
  delay(200); //change this delay of you don't want the http get to be called so often
}

void changeColor(String rgbString)
{
     
      //Parse RGB;
      int rIndex=rgbString.indexOf(',');
      int gIndex=rgbString.indexOf(',', rIndex+1);

      int r= rgbString.substring(0,rIndex).toInt();
      int g= rgbString.substring(rIndex+1,gIndex).toInt();
      int b= rgbString.substring(gIndex+1).toInt();
      
      //map to the maximum intensity available on the infineon shield
      r=map(r,0,255,0,4095);
      g=map(g,0,255,0,4095);
      b=map(b,0,255,0,4095);
      
      
      rgbShield.setRGB(r,g,b);
}
