/*  ----------- KEYPAD CODE


#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

//define the two-dimensional array on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {18, 19, 21, 22}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {27,26,25,33}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  Serial.begin(9600);
}
  
void loop(){
  char customKey = customKeypad.getKey();
  
  if (customKey){
    Serial.println(customKey);
  }
}


*/



/*
  ----------- LED BLINK CODE

// Configure GPIO pins
const int RedLedPin = 4;  
const int GreenLedPin = 16;  
const int YellowLedPin = 17;  

void setup() {
  pinMode(RedLedPin, OUTPUT);
  pinMode(GreenLedPin, OUTPUT);
  pinMode(YellowLedPin, OUTPUT);
}

void loop() {
  digitalWrite(RedLedPin, HIGH);  
  digitalWrite(GreenLedPin, HIGH);
  digitalWrite(YellowLedPin, HIGH);
  delay(1000);                 // Wait 1 second (1000 ms)
  digitalWrite(RedLedPin, LOW); 
  digitalWrite(GreenLedPin, LOW);
  digitalWrite(YellowLedPin, LOW);  
  delay(1000);                 // Wait 1 second (1000 ms)
}

*/

#include <WiFi.h>
#include <HTTPClient.h>
#include "secrets.h"


void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected");

  HTTPClient http;
  http.begin(url);

  int statusCode = http.GET();
  if (statusCode > 0) {
    Serial.printf("HTTP status: %d\n", statusCode);
    Serial.println("Response:");
    Serial.println(http.getString());
  } else {
    Serial.printf("GET failed: %s\n", http.errorToString(statusCode).c_str());
  }

  http.end();
}

void loop() {
}