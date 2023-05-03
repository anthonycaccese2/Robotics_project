/*
  Esp32 Motor controller 

*/
#include <ESP32Servo.h>
#include <esp_now.h>
#include <WiFi.h>
#include "CytronMotorDriver.h"
// Steering servo
Servo myservo;  // create servo object to control a servo

#define BUTTON_PIN 21 // GIOP21 pin connected to button
#define RELAY_PIN 16 // ESP32 pin GIOP16 connected to the IN pin of relay
#define DRMOTOR_PWM 27
#define DRMOTOR_DIR 26
#define SERVO 17

bool motorPowerState = false;

// Configure the motor driver.
CytronMD motor(PWM_DIR, DRMOTOR_PWM, DRMOTOR_DIR);  // PWM = Pin 3, DIR = Pin 4.

// WIFI 

uint8_t broadcastAddress[] = {0xC8,0xC9,0xA3,0xC5,0xBF,0xEC};

// Variable to store if sending data was successful
String success;

// Define variables to store incoming readings
float currentTurn = 0;
int currentSpeed = 0; // -255 to 255
bool currentDir = false; // 0 forward; 1 backward

typedef struct struct_message {
    int turn;
    int speed;
    bool dir;
} struct_message;

// Create a struct_message to hold incoming sensor readings
struct_message incomingReadings;

esp_now_peer_info_t peerInfo;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0){
    success = "Delivery Success :)";
  }
  else{
    success = "Delivery Fail :(";
  }
}

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  currentTurn = incomingReadings.turn;
  currentSpeed = incomingReadings.speed;
  currentDir = incomingReadings.dir;
//  Serial.print("incomingTurn: ");
//  Serial.println(currentTurn);
//  Serial.print("incomingSpeed: ");
//  Serial.println(currentSpeed);
//  Serial.print("incomingDir: ");
//  Serial.println(currentDir);
}


float floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void setup() {
    ESP32PWM::allocateTimer(0);
    Serial.begin(115200);

    pinMode(BUTTON_PIN, INPUT_PULLUP); // button
    // initialize digital pin as an output.
    pinMode(RELAY_PIN, OUTPUT);

    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);
  
    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
      Serial.println("Error initializing ESP-NOW");
      return;
    }
  
    // Once ESPNow is successfully Init, we will register for Send CB to
    // get the status of Trasnmitted packet
    esp_now_register_send_cb(OnDataSent);
    
    // Register peer
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;
    
    // Add peer        
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
      Serial.println("Failed to add peer");
      return;
    }
    // Register for a callback function that will be called when data is received
    esp_now_register_recv_cb(OnDataRecv);

  //Steering servo 
    myservo.attach(17, 500, 2500);  // attaches the servo on pin 9 to the servo object
    myservo.setPeriodHertz(50);      // Standard 50hz servo
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
      float tempSpeed = floatMap(currentSpeed, 0, 20.0, 0, 225);
      if (currentDir) {
        tempSpeed = -tempSpeed;
      }
      motor.setSpeed(tempSpeed);
      float tempTurn = floatMap(currentTurn, -90, 90, 85, 105);
      Serial.print("Turn:");
      Serial.println(tempTurn);
      myservo.write(tempTurn);
      
    } else {
      Serial.println("Motor false");
      motor.setSpeed(0);
      float tempTurn = 90;
      Serial.println(tempTurn);
      myservo.write(90);
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
