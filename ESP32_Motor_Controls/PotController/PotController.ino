/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-potentiometer
 */
#include <esp_now.h>
#include <WiFi.h>

// WIFI
uint8_t broadcastAddress[] = {0x34,0x94,0x54,0x25,0x1E,0xE0};

// Variable to store if sending data was successful
String success;

typedef struct struct_message {
    int turn;
    int speed;
    bool dir;
} struct_message;

// Create a struct_message called BME280Readings to hold sensor readings
struct_message dataReadings;

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
//
//// Callback when data is received
//void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
//  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
//  Serial.print("Bytes received: ");
//  Serial.println(len);
//  incomingTemp = incomingReadings.temp;
//  incomingHum = incomingReadings.hum;
//  incomingPres = incomingReadings.pres;
//}



// Pots 
const int turnPin = 39;// Analog pin
const int speedPin = 36;// Analog pin
const int dirPin = 21;// Switch pin

int turnValue;// do not change
float voltage =0;// do not change

int speedValue;// do not change
float voltage1 =0;// do not change

bool dirValue; 


float floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  pinMode(dirPin, INPUT_PULLUP); // button

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
//  esp_now_register_recv_cb(OnDataRecv);
}

// the loop routine runs over and over again forever:
void loop() {
// read the input on analog pin potPin:
  voltage = (3.3/4095.0) * turnValue;
  turnValue = analogRead(turnPin);
  turnValue = floatMap(turnValue, 0, 4095.0, -90, 90); 
  
  speedValue = analogRead(speedPin);
  voltage1 = (3.3/4095.0) * speedValue;
  speedValue = floatMap(speedValue, 0, 4095.0, 0, 20); 

  int buttonState = digitalRead(dirPin);
   if (buttonState==0){
    dirValue = false;
  } else {
    dirValue = true;
  }
  // prepared data 
  dataReadings.turn = turnValue;
  dataReadings.speed = speedValue;
  dataReadings.dir = dirValue;
 
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &dataReadings, sizeof(dataReadings));
   
  if (result == ESP_OK) {
    Serial.print("Sent with success");
    Serial.println(dirValue);
    
  }
  else {
    Serial.println("Error sending the data");
  }
//  Serial.print("potValue:");
//  Serial.print(potValue);
//  
//  Serial.print("potValue1:");
//  Serial.print(potValue1);
//   
//  Serial.print(" Voltage:");
//  Serial.print(voltage);
//  Serial.println("V");  
  delay(50); // delay in between reads for stability
}
