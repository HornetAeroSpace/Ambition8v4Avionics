#include <Arduino.h>

unsigned long lastTime = 0; // Stores the last time information was printed
const long interval =1000; // Interval at which to print information (1000 milliseconds = 1 seconds)
int data1 = 255;
int data2 = 0;
float data3 = -9.81;

void setup() {
  Serial.begin(115200); // Start serial communication at 115200 baud
  Serial.println("ESP32 Serial Monitor Test");
}

void loop() {
  unsigned long currentTime = millis();

  // Check if it's time to print the information again
  if (currentTime - lastTime >= interval) {
    lastTime = currentTime; // Update the last time information was printed

    // Print system uptime
    //Serial.print("System uptime (ms): ");
    //Serial.println(currentTime);
    //Serial.println("\n");

    // Print data1 int
    //Serial.print("data1 = ");
    Serial.print(data1);
    Serial.print(",");
    Serial.print(data2);
    Serial.print(",");
    Serial.print(data3);
    Serial.print(",");
    Serial.println();
    //Serial.println("\n");

  }
}
