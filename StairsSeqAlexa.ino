
#include <Arduino.h>
//#include <ESP8266WiFi.h>
//#include "fauxmoESP.h"

#define SERIAL_BAUDRATE 115200

int relayPins[] = {16, 5, 4, 0, 2, 14, 12, 13, 15};       // an array of pin numbers to which relays are attached
int pinCount = 9;  // the number of pins (i.e. the length of the array)
const int motion1 = 10; // pir sensors pins
const int motion2 = 9;

void setup() {
  // put your setup code here, to run once:
   pinMode(motion1, INPUT);
   pinMode(motion2, INPUT);
  
for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    pinMode(relayPins[thisPin], INPUT_PULLUP);
    pinMode(relayPins[thisPin], OUTPUT);//shuold default as HIGH so relays off
   // digitalWrite(relayPins[thisPin], HIGH); //start as relay off if connected to NO
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  int sensor1 = digitalRead(motion1); //declaring variable to hold sensor data
  int sensor2 = digitalRead(motion2);
  
  if(sensor1 == HIGH && sensor2 == HIGH){ //unlikely scenario
        for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    digitalWrite(relayPins[thisPin], LOW); //  relays all on
        }
        delay(60000);
         for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    digitalWrite(relayPins[thisPin], HIGH); //  relays all off
        }
  }else if(sensor1 == HIGH && sensor2 == LOW){ //bottom sensor
    Serial.println("Bottom sensor triggered"); 
    for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    digitalWrite(relayPins[thisPin], LOW); // energize relays until all on
    delay(500);
  }
  delay(50000); // stay on for 50 seconds going up
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
  digitalWrite(relayPins[thisPin], HIGH); // de-energize relays until all off
  delay(500);
  }
  delay(50); // little delay to avoid bouncing
  }else if(sensor1 == LOW && sensor2 == HIGH){ //top to bottom
    Serial.println("Top sensor triggered");
    for (int thisPin = 8; thisPin >= 0; thisPin--) {
    digitalWrite(relayPins[thisPin], LOW); // energize relays until all on
    delay(500);
  }
  delay(70000); // stay on for 70 seconds going down
  for (int thisPin = 8; thisPin >= 0; thisPin--){
  digitalWrite(relayPins[thisPin], HIGH); // de-energize relays until all off
  delay(500);
  }
  delay(50); // little delay to avoid bouncing
    }else{
    for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    digitalWrite(relayPins[thisPin], HIGH); //  relays all off
   // delay(1000);
   }
  }

}

