#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress[] = {0xC8,0xC9,0xA3,0xC5,0xBF,0xEC};

// Variable to store if sending data was successful
String success;

// Define variables to store incoming readings
float incomingTurn;
float incomingSpeed;
bool incomingDir;

typedef struct struct_message {
    int turn;
    int speed;
    bool dir;
} struct_message;

// Create a struct_message called BME280Readings to hold sensor readings
struct_message BME280Readings;

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
  incomingTurn = incomingReadings.turn;
  incomingSpeed = incomingReadings.speed;
  incomingDir = incomingReadings.dir;
  Serial.print("incomingTurn: ");
  Serial.println(incomingTurn);
  Serial.print("incomingSpeed: ");
  Serial.println(incomingSpeed);
  Serial.print("incomingDir: ");
  Serial.println(incomingDir);
}

void setup() {
  // put your setup code here, to run once:
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
//  WiFi.mode(WIFI_MODE_STA);
  Serial.println(WiFi.macAddress());

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

}

void loop() {
  // put your main code here, to run repeatedly:
  
//  getReadings();
}
