#include <Wire.h>
#include <Adafruit_TCS34725.h>  //Adafruit_TCS34735.h caused compile error. so removed the underscore.
#include <SoftwareSerial.h>
#include <avr/pgmspace.h>
#include "gamma256.h"

#define FLEXPIN A0
#define ESPRXPIN 4
#define ESPTXPIN 5
#define LEDPINR 9
#define LEDPING 10
#define LEDPINB 11
#define _DEBUG
#define ESP_RX_PIN 4
#define ESP_TX_PIN 5

 extern const uint8_t gamma[];
//Storing in PROGMEM to save dynamic memory space.
//Not the topic name (uri after PUT) and the host name. Change it to your topic and hostname:port.
  PROGMEM  const  char  tcp_packet_template[] ="PUT /resources/FromLightGlove/RGB HTTP/1.1\r\n"
                                       "Host: 192.168.1.217:6000\r\n"
                                      "Accept: */*\r\n"
                                      "Content-Type: application/x-www-form-urlencoded\r\n" ;
 
 //the server name should be surronded by double quote.. so pay attention to the escape charcter for double quote
  const char startTCPSession[] = "AT+CIPSTART=\"TCP\",\"192.168.1.217\",6000"; 

Adafruit_TCS34725 colorSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
SoftwareSerial esp8266Serial(ESP_RX_PIN, ESP_TX_PIN);

//Color Capture flags
boolean colorCaptureStarted=false;
boolean colorCaptureFinished=false;
char capturedColorRGB[12];

void setup() {
    Serial.begin(9600);
    colorSensor.begin();
    esp8266Serial.begin(9600);
     esp8266Serial.setTimeout(1000);
     
       pinMode(LEDPINR, OUTPUT);
  pinMode(LEDPING, OUTPUT);
  pinMode(LEDPINB, OUTPUT);
 captureColor();  //do it to turn off the LED and as a sanity check
 //  colorSensor.setInterrupt(true);  // turn off LED
  
}

void loop() {
  
  //Straight value 770
  //Bent 90 degree value 870
  int bentDegrees;
  int rawFlex=analogRead(FLEXPIN);
  bentDegrees=map(rawFlex,770,870,0,90);
  Serial.println(rawFlex);
  Serial.println(bentDegrees);
  
  if(bentDegrees >80 && !colorCaptureStarted)
  {
    Serial.println("Bent..Initiate Color Capture");
    colorCaptureStarted=true;
    captureColor();
    colorCaptureFinished=true;
  }
  
  if(bentDegrees< 15) //throw color if captured
  {
    if(colorCaptureStarted && colorCaptureFinished)
    {
     
      SendMQTTMessage(capturedColorRGB);
      //reset after throwing
      colorCaptureStarted=false;
      colorCaptureFinished=false;
 
    }
  }
  delay(100);
}

void SendMQTTMessage(char* content)
{
    //make sure you adjust this if the template stored in progmem changes depending on your
    //host name, port, topic etc.
   char tcpPacketTemplateBuffer[256];
   strcpy_P(tcpPacketTemplateBuffer,tcp_packet_template);
    
    int contentLength=strlen(content);
 
    sprintf(tcpPacketTemplateBuffer,"%sContent-Length: %d\r\n\r\n%s", tcpPacketTemplateBuffer,contentLength,content);
    
    esp8266Serial.println(startTCPSession);
    
    if (esp8266Serial.find("Linked"))
    {
        esp8266Serial.print("AT+CIPSEND=");
        esp8266Serial.println(strlen(tcpPacketTemplateBuffer));

        esp8266Serial.println(tcpPacketTemplateBuffer);
 
        if (esp8266Serial.find("SEND OK"))
        {
          //After reading close the connection. Do not want to reuse because the TCP stack on
          //ESP8266 might close the connection after a while.
          //So make sure the connection is closed by sending AT+CIPCLOSE
          esp8266Serial.println("AT+CIPCLOSE");
          
          Serial.println("Message Sent.");
        } else
        {
    
          Serial.println("Sending Packet Failed");
        }
  } else
  {
    Serial.println("Cannot Start TCP Session");
   }    
    
   // Serial.println(tcpPacketTemplateBuffer);
   // Serial.println(strlen(tcpPacketTemplateBuffer));
}

void captureColor()
{
  uint16_t clear, red, green, blue;
  memset(capturedColorRGB,0,12);
 
  colorSensor.setInterrupt(false);      // turn on LED
 
  delay(60);  // takes 50ms to read 
  
  colorSensor.getRawData(&red, &green, &blue, &clear);
 
  colorSensor.setInterrupt(true);  // turn off LED
  
  // Figure out some basic hex code for visualization
  uint32_t sum = red+green+blue;
 
  float r, g, b;
  r = red; r /= sum;
  g = green; g /= sum;
  b = blue; b /= sum;
  r *= 256; g *= 256; b *= 256;
   if (r > 255) r = 255;
  if (g > 255) g = 255;
  if (b > 255) b = 255;
  
  sprintf(capturedColorRGB,"%d,%d,%d", (int)r ,(int)g ,(int)b) ;
  setColor( (int)r ,(int)g ,(int)b);
}

void setColor(int red, int green, int blue)
{
  analogWrite(LEDPINR, pgm_read_byte(&gamma[red]));
  analogWrite(LEDPING, pgm_read_byte(&gamma[green]));
  analogWrite(LEDPINB, pgm_read_byte(&gamma[blue]));  
}

