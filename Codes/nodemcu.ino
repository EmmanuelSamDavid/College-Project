
#include <Wire.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include "Pushover.h"
const char* ssid = "iotdata";
const char* password = "Indianss@12";
unsigned long myChannelNumber =  2529961;
const char * myWriteAPIKey = "XI5ZNRO5JCVH4N5Q";
const char* pushoverToken = "ajaj5bmyxoft84zwtydrqfej5hmdkj";
const char* pushoverUserKey = "u5zqsfu7tfombstw18hx6g6tv1nav3";
WiFiClient  client;
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is completelag
unsigned int spacePosition=0;
String gas_status="";
String level_status="";
String level_value="";
unsigned int count=0;
void sendPushoverNotification(bool gasDetected,bool levelflag, float level) {
  // Initialize Pushover
  Pushover po = Pushover(pushoverToken, pushoverUserKey, UNSAFE);

  if (gasDetected) {
    // Create message with clickable Google Maps link
    String message = "Gas Leakage detected!";

    // Send Pushover notification
    po.setMessage(message);
    po.send();
  }
  if (levelflag) {
    // Create message with clickable Google Maps link
    String message = "Gas Level Near Empty!Automatic Booking Initiated!Status:Processing";

    // Send Pushover notification
    po.setMessage(message);
    po.send();
  }
    ThingSpeak.setField(1, gasDetected);
  ThingSpeak.setField(2,levelflag);
  ThingSpeak.setField(3, level);
     int m = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  
}

void setup()
{
  Serial.begin(115200);
  inputString.reserve(200);
  gas_status.reserve(10);
 level_status.reserve(10);
  level_value.reserve(10);

  ThingSpeak.begin(client);
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected");
}
 
void loop()
{
  while (Serial.available()) 
  {
    // get the new byte:up
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') 
    {
         stringComplete=true;
    } 
  
  }
     if(stringComplete)
     {
         spacePosition=inputString.indexOf("&field1=");
         spacePosition+=8;
               gas_status=inputString[spacePosition++];
         spacePosition=inputString.indexOf("&field2=");
         spacePosition+=8;
               level_status=inputString[spacePosition++];
           spacePosition=inputString.indexOf("&field3=");
             spacePosition+=8;
           for(count=0;count<6;count++)
            {
               level_value+=inputString[spacePosition++];
            }
                  }
bool gasDetected = (gas_status.charAt(0) == '1');
bool levelflag = (level_status.charAt(0) == '1');
float level = level_value.substring(0, 12).toFloat();  // Assuming latitude is in the format xx.xxxxxx

    sendPushoverNotification(gasDetected, levelflag, level);

    gas_status="";
   level_status="";
   level_value="";

    
//    flow_value="";
//    vol_value="";
//    x_value="";
//    y_value="";
//    z_value="";
        inputString = "";
       stringComplete=false;
 delay(1000);
}
