#include <WiFi.h>
#include <HTTPClient.h>
#include "secrets.h"
#include <Keypad.h>


// Configure GPIO pins
const int RedLedPin = 4;  
const int GreenLedPin = 16;  
const int YellowLedPin = 17;

// Configure keypad
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

// Store user input from the keypad
String inputAccessCode;

// Function to send HTTP GET request to check the access code
void sendRequest(String accessCode) {
  HTTPClient http;
  http.begin(url+accessCode);

  int statusCode = http.GET();
  if (statusCode == 200) {
    Serial.printf("GET success:");
    // Access code is correct, turn on the green LED for 5 seconds
    digitalWrite(RedLedPin, LOW);
    digitalWrite(GreenLedPin, HIGH);
    delay(5000);
    digitalWrite(GreenLedPin, LOW);
    digitalWrite(RedLedPin, HIGH);
  } else {
    Serial.printf("GET failed: %s\n", http.errorToString(statusCode).c_str());
    // Access code is incorrect, turn on the yellow LED for 5 seconds
    digitalWrite(RedLedPin, LOW);
    digitalWrite(YellowLedPin, HIGH);
    delay(5000);
    digitalWrite(YellowLedPin, LOW);
    digitalWrite(RedLedPin, HIGH);
  }
  Serial.println();

  http.end();
}

void setup() {
  Serial.begin(9600);

  // configure the LEDs and start with the RED LED on
  pinMode(RedLedPin, OUTPUT);
  pinMode(GreenLedPin, OUTPUT);
  pinMode(YellowLedPin, OUTPUT);

  digitalWrite(RedLedPin, HIGH);

  // Connect esp32 to Wi-Fi 
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected");  
}

void loop() {
  // Read the keypad input
  char customKey = customKeypad.getKey();
  if (customKey != NO_KEY) {
    if (customKey == '#') {  
      // # submits the code
      Serial.println(inputAccessCode);

      sendRequest(inputAccessCode);

      inputAccessCode = "";  // clear for the next entry
    } else if (customKey == '*') {  
      // * clears the input
      inputAccessCode = "";
    } else {
      inputAccessCode += customKey;
    }
  }
}