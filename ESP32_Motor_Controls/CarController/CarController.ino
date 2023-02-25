/*
  Esp32 Websockets Client

  This sketch:
        1. Connects to a WiFi network
        2. Connects to a Websockets server
        3. Sends the websockets server a message ("Hello Server")
        4. Prints all incoming messages while the connection is open

  Hardware:
        For this sketch you only need an ESP32 board.

  Created 15/02/2019
  By Gil Maimon
  https://github.com/gilmaimon/ArduinoWebsockets

*/

#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include "CytronMotorDriver.h"

const char* ssid = "ssid"; //Enter SSID
const char* password = "password"; //Enter Password
const char* websockets_server_host = "serverip_or_name"; //Enter server adress
const uint16_t websockets_server_port = 8080; // Enter server port

#define BUTTON_PIN 21 // GIOP21 pin connected to button
#define RELAY_PIN 16 // ESP32 pin GIOP16 connected to the IN pin of relay
#define DRMOTOR_PWM 27
#define DRMOTOR_DIR 26


bool motorPowerState = false;
int currentSpeed = 128; // -255 to 255



// Configure the motor driver.
CytronMD motor(PWM_DIR, DRMOTOR_PWM, DRMOTOR_DIR);  // PWM = Pin 3, DIR = Pin 4.

using namespace websockets;

WebsocketsClient client;
void setup() {
    Serial.begin(115200);
    // Connect to wifi
//    WiFi.begin(ssid, password);

    pinMode(BUTTON_PIN, INPUT_PULLUP); // button
    // initialize digital pin as an output.
    pinMode(RELAY_PIN, OUTPUT);

//
//    // Wait some time to connect to wifi
//    for(int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
//        Serial.print(".");
//        delay(1000);
//    }
//
//
//    // Check if connected to wifi
//    if(WiFi.status() != WL_CONNECTED) {
//        Serial.println("No Wifi!");
//        return;
//    }
//
//    Serial.println("Connected to Wifi, Connecting to server.");
//    // try to connect to Websockets server
//    bool connected = client.connect(websockets_server_host, websockets_server_port, "/");
//    if(connected) {
//        Serial.println("Connected!");
//        client.send("Hello Server");
//    } else {
//        Serial.println("Not Connected!");
//    }
//    
//    // run callback when messages are received
//    client.onMessage([&](WebsocketsMessage message){
//        Serial.print("Got Message: ");
//        Serial.println(message.data());
//    });
}

void loop() {
    // let the websockets client check for incoming messages
      
    // read the state of the switch/button:
    int buttonState = digitalRead(BUTTON_PIN);
    // print out the button's state
    Serial.println(buttonState);
    if (buttonState == 1){ // power on
        digitalWrite(RELAY_PIN, LOW);
//        motor.setSpeed(128);
//       motor.setSpeed(currentSpeed);
      if(motorPowerState){ // motors are on
       // do motor stuff
      } else {
        motorPowerState = true;
        Serial.println("Switching Motor on"); 
      }
    } else { // power off
        digitalWrite(RELAY_PIN, HIGH);
      if(motorPowerState){
        motorPowerState = false;
        Serial.println("Switching Motor off");  // switch motor off
//        motor.setSpeed(0);
      } else {
//        motor.setSpeed(0);
      }
    }
    if(motorPowerState){
      Serial.println("Motor true");
      motor.setSpeed(128);
    } else {
      Serial.println("Motor false");
      motor.setSpeed(0);
    }
//    
//    if(client.available()) { // WSS
//        client.poll();
//    }
    delay(100);
}

/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/
//
//// Motor A
//int motor1Pin1 = 27; 
//int motor1Pin2 = 26; 
//int enable1Pin = 14; 
//
//// Setting PWM properties
//const int freq = 30000;
//const int pwmChannel = 0;
//const int resolution = 8;
//int dutyCycle = 200;
//
//void setup() {
//  // sets the pins as outputs:
//  pinMode(motor1Pin1, OUTPUT);
//  pinMode(motor1Pin2, OUTPUT);
//  pinMode(enable1Pin, OUTPUT);
//  
//  // configure LED PWM functionalitites
//  ledcSetup(pwmChannel, freq, resolution);
//  
//  // attach the channel to the GPIO to be controlled
//  ledcAttachPin(enable1Pin, pwmChannel);
//
//  Serial.begin(115200);
//
//  // testing
//  Serial.print("Testing DC Motor...");
//}
//
//void loop() {
//  // Move the DC motor forward at maximum speed
//  Serial.println("Moving Forward");
//  digitalWrite(motor1Pin1, LOW);
//  digitalWrite(motor1Pin2, HIGH); 
//  delay(2000);
//
//  // Stop the DC motor
//  Serial.println("Motor stopped");
//  digitalWrite(motor1Pin1, LOW);
//  digitalWrite(motor1Pin2, LOW);
//  delay(1000);
//
//  // Move DC motor backwards at maximum speed
//  Serial.println("Moving Backwards");
//  digitalWrite(motor1Pin1, HIGH);
//  digitalWrite(motor1Pin2, LOW); 
//  delay(2000);
//
//  // Stop the DC motor
//  Serial.println("Motor stopped");
//  digitalWrite(motor1Pin1, LOW);
//  digitalWrite(motor1Pin2, LOW);
//  delay(1000);
//
//  // Move DC motor forward with increasing speed
//  digitalWrite(motor1Pin1, HIGH);
//  digitalWrite(motor1Pin2, LOW);
//  while (dutyCycle <= 255){
//    ledcWrite(pwmChannel, dutyCycle);   
//    Serial.print("Forward with duty cycle: ");
//    Serial.println(dutyCycle);
//    dutyCycle = dutyCycle + 5;
//    delay(500);
//  }
//  dutyCycle = 200;
//}
