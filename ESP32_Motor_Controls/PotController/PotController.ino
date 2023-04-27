/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-potentiometer
 */

#include "WiFi.h"

float floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
const int potPin = 39;// Analog pin
const int potPin1 = 36;// Analog pin

int potValue;// do not change
float voltage =0;// do not change

int potValue1;// do not change
float voltage1 =0;// do not change

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  WiFi.mode(WIFI_MODE_STA);
  Serial.println(WiFi.macAddress());
}

// the loop routine runs over and over again forever:
void loop() {
// read the input on analog pin potPin:
  voltage = (3.3/4095.0) * potValue;
  potValue = analogRead(potPin);
  potValue = floatMap(potValue, 0, 4095.0, -90, 90); 
  
  potValue1 = analogRead(potPin1);
  voltage1 = (3.3/4095.0) * potValue1;
  potValue1 = floatMap(potValue1, 0, 4095.0, -90, 90); 
  Serial.print("potValue:");
  Serial.print(potValue);
  
  Serial.print("potValue1:");
  Serial.print(potValue1);
   
  Serial.print(" Voltage:");
  Serial.print(voltage);
  Serial.println("V");  
  delay(50); // delay in between reads for stability
}
