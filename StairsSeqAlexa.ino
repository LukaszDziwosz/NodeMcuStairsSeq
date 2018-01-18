// LED stairs sequence by Lukasz Dziwosz 2018 for NodeMCU coded in Arduino IDE
// Amazon Alexa enabled, 2 PIR(motion) sensors

#include <Arduino.h>
#include <ESP8266WiFi.h> //enabling wifi on NodeMCU
#include "fauxmoESP.h"

/* Network credentials */
#define WIFI_SSID "YOURNETWORK"
#define WIFI_PASS "WIFIPASSWORD"  

/* Serial Monitor*/
#define SERIAL_BAUDRATE 115200

/* Belkin WeMo emulation */
fauxmoESP fauxmo;

/* Set Relay Pins and PIR pins*/
int relayPins[] = {16, 5, 4, 0, 2, 14, 12, 13, 15};       // an array of pin numbers to which relays are attached
int pinCount = 9;  // the number of pins (i.e. the length of the array)

const int motion1 = 10; // pir sensors pins
const int motion2 = 9;

void setup() {
  
Serial.begin(SERIAL_BAUDRATE);
// put your setup code here, to run once:

//setup and wifi connection
 wifiSetup();

//Set PIR pins to inputs 
 pinMode(motion1, INPUT);
 pinMode(motion2, INPUT);

//Set relay pins to outputs
 for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    pinMode(relayPins[thisPin], INPUT_PULLUP);
    pinMode(relayPins[thisPin], OUTPUT);
    digitalWrite(relayPins[thisPin], HIGH); //start as relay off if connected to NO(normally open)
  }

// Device Names for Simulated Wemo switches
 fauxmo.addDevice("Stairs");

 fauxmo.onMessage(callback);   

}

void loop() {
// put your main code here, to run repeatedly:

 //fauxmo.handle();

 int sensor1 = digitalRead(motion1); //declaring variable to hold sensor data
 int sensor2 = digitalRead(motion2);

 if(sensor1 == HIGH && sensor2 == LOW){ //bottom sensor
    Serial.println("Bottom sensor triggered"); 
    sequenceUp();
  }else if(sensor1 == LOW && sensor2 == HIGH){ //top to bottom
    Serial.println("Top sensor triggered");
    sequenceDown();
    }else{
 fauxmo.handle();
  }
}
/* --------------------------------------------------------------------------- 
Light Sequence Methods
----------------------------------------------------------------------------*/ 
void sequenceUp(){
 for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    digitalWrite(relayPins[thisPin], LOW); // energize relays until all on
    delay(250);}
 delay(40000); // stay on for 10 seconds going up
 for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    digitalWrite(relayPins[thisPin], HIGH); // de-energize relays until all off
    delay(250);}
  delay(50); // little delay to avoid bouncing  
}
void sequenceDown(){
  for (int thisPin = 8; thisPin >= 0; thisPin--) {
    digitalWrite(relayPins[thisPin], LOW); // energize relays until all on
    delay(250);
  }
  delay(40000); // stay on for 10 seconds going down
  for (int thisPin = 8; thisPin >= 0; thisPin--){
  digitalWrite(relayPins[thisPin], HIGH); // de-energize relays until all off
  delay(250);
  }
  delay(50); // little delay to avoid bouncing
}

/* --------------------------------------------------------------------------- 
Device Callback (Alexa calling)
----------------------------------------------------------------------------*/ 
void callback(uint8_t device_id, const char * device_name, bool state)  
{ 
 Serial.print("Device "); Serial.print(device_name);  
 Serial.print(" state: "); 
 if (state)  
 { 
   Serial.println("ON"); 
   for (int thisPin = 0; thisPin < pinCount; thisPin++) {
   digitalWrite(relayPins[thisPin], LOW); 
    } 
 }  
 else  
 { 
   Serial.println("OFF"); 
   for (int thisPin = 0; thisPin < pinCount; thisPin++) {
   digitalWrite(relayPins[thisPin], HIGH); 
        } 
 } 
 
}
/* -----------------------------------------------------------------------------
 Wifi Setup
 -----------------------------------------------------------------------------*/
void wifiSetup() 
{
   // Set WIFI module to STA mode
   WiFi.mode(WIFI_STA);

   // Connect
   Serial.println ();
   Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
   Serial.println();
   WiFi.begin(WIFI_SSID, WIFI_PASS);

   // Wait
   while (WiFi.status() != WL_CONNECTED) 
   {
      Serial.print(".");
      delay(100);
   }
   Serial.print(" ==> CONNECTED!" );
   Serial.println();

   // Connected!
   Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
   Serial.println();
   
}
