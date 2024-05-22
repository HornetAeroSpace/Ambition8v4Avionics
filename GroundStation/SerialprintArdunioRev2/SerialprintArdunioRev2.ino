#include <Arduino.h>
#include <math.h>

// Function Declarations:

void DataCOTS1();
void DataSARD1();
void DataSARD2();

unsigned long lastTime = 0;        // Stores the last time information was printed
const long sampleInterval = 250;  // Interval at which to print information (1000 milliseconds = 1 seconds)

// Sample Data Variables
float t = 0.0;
float a = 0.1;
float v = 0.0;
float z = 0.0;
float ang = 0.0;
const int r = 10;
const int endTime = 25000;  // End sim at 60 sec
bool portStatus = true;
bool ctrlStatus = true;

void setup() {
  Serial.begin(115200);  // Start serial communication at 115200 baud
  //Serial.println("ESP32 Serial Monitor Test");
  delay(5000);  // Delay to allow COM port to be opened in MatLab
}


void loop() {
  unsigned long currentTime = millis() - 5000;  // Removed delay so sim data starts at zero


  // Check if it's time to print the information again
  if (currentTime - lastTime >= sampleInterval && currentTime < endTime) {
    lastTime = currentTime;  // Update the last time information was printed

    // Generation of some data to transmit
    t = (millis() - 5000) / 1000;
    v = (a * t) + v;
    z = (0.5 * a * sq(t)) + (v * t) + z;
    ang = (millis() - 5000);

    // Printing of data in serial
    // Time
    DataCOTS1(ang, r, t);
    DataSARD1(a, v, z);
    DataSARD2(endTime, portStatus, ctrlStatus);
    Serial.println("end"); // println inculdes CR and LF terminators \r\n respectively



  } 
  if (currentTime - lastTime >= sampleInterval && currentTime > endTime) {
    lastTime = currentTime;  // Update the last time information was printed
    Serial.println("End of Sim");
  }
}



void DataCOTS1(float ang, float r, float t) {
  // Print Device name before data stream
  Serial.print("DataCOTS1:");
  Serial.print(",");
  //Data stream code, data seperated by commas end by serial println
  Serial.print(ang);
  Serial.print(",");
  // Velocity
  Serial.print(r);
  Serial.print(",");
  // Hight in z
  Serial.print(t);
  //Serial.print(",");
  Serial.println();
}

void DataSARD1(float a, float v, float z) {
  // Print Device name before data stream
  Serial.print("DataSARD1:");
  Serial.print(",");
  //Data stream code, data seperated by commas end by serial println
  Serial.print(a);
  Serial.print(",");
  // Velocity
  Serial.print(v);
  Serial.print(",");
  // Height in z
  Serial.print(z);
 // Serial.print(",");
  Serial.println();
}

void DataSARD2(int endtime, bool portStatus, bool ctrlStatus) {
  // Print Device name before data stream
  Serial.print("DataSARD2:");
  Serial.print(",");
  //Data stream code, data seperated by commas end by serial println
  Serial.print(endtime);
  Serial.print(",");
  // Velocity
  Serial.print(portStatus);
  Serial.print(",");
  // Hight in z
  Serial.print(ctrlStatus);
  //Serial.print(",");
  Serial.println();
}