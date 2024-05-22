#include <Arduino.h>
#include <math.h>
unsigned long lastTime = 0; // Stores the last time information was printed
const long sampleInterval =1000; // Interval at which to print information (1000 milliseconds = 1 seconds)

// Sample Data Variables
float t = 0.0; 
float a = 0.1;
float v = 0.0;
float z  = 0.0;
float ang = 0.0;
const int r = 10;
const int endTime = 60000; // End sim at 60 sec

void setup() {
  Serial.begin(115200); // Start serial communication at 115200 baud
  //Serial.println("ESP32 Serial Monitor Test");
  delay(5000); // Delay to allow COM port to be opened in MatLab
}

void loop() {
  unsigned long currentTime = millis() - 5000; // Removed delay so sim data starts at zero


  // Check if it's time to print the information again
  if (currentTime - lastTime >= sampleInterval && currentTime < endTime) {
    lastTime = currentTime; // Update the last time information was printed

    // Generation of some data to transmit
    t = (millis()-5000) / 1000;
    v = (a * t) + v;
    z = (0.5 * a *sq(t)) + (v * t) + z;
    ang = (millis() - 5000);

    // Printing of data in serial
    // Time
    Serial.print(t);
    Serial.print(",");
    // Aceleration
    Serial.print(a);
    Serial.print(",");
    // Velocity
    Serial.print(v);
    Serial.print(",");
    // Hight in z
    Serial.print(z);
    Serial.print(",");
    // Other postion data
    Serial.print(ang);
    Serial.print(",");
    Serial.print(r);
    Serial.print(",");
    // End of transmit
    Serial.println(); // println inculdes CR and LF terminators \r\n respectively 

  }
  else{
    //Serial.println();
  }
}
