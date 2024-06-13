#include <SPI.h>
//#include <LoRa.h>
#include "Arduino.h"
#include <Wire.h>
#include <string>
//#include <SD.h> 
#include <math.h>

// ** Necessary dependencies to download

#include "Adafruit_ICM20948.h"
#include "Adafruit_ICM20X.h"
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
#include "Adafruit_ADXL375.h"
#include <Wire.h>



#define ICM_SDA 17
#define ICM_SCL 16

#define BMP_SDA 18
#define BMP_SCL 19

#define ACC_SDA 25
#define ACC_SCL 24

#define LORA_BAND 433000000

#define SS 10
#define RST 32
#define DIO 26


#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BMP3XX bmp;
Adafruit_ICM20948 icm;
Adafruit_ADXL375 accel375 = Adafruit_ADXL375(5,&Wire2);

float getSeconds(long time){
  return float(abs((time/1000)-(round(time/60000)*60)));
}

// float getMinutes(long time){
//   return float(round(time/60000));
// }

long currT;
float tempAk[2] = {0,0};
//double tempAlt[2] = {0,0};
int stepAk = 0;
//int stepAlt = 0;
//temp updates to have 2ndLast/Last cells, swapping alternations, 2nd cycle last/2nd last - IN THEORY!
void sandDropAk(float delta, int flip, int invFlp){
  float linDif = delta - tempAk[flip];
  float linPas = tempAk[flip] - tempAk[invFlp];
    Serial.println("linPas, linDif: "+String(linPas)+","+String(linDif));
  if(linPas*linDif <= 0){
    stepAk = 0;
    return;
  }else{
    stepAk = stepAk+1;
    return;
  }
  Serial.println("SndDropAk No If");
}

// void sandDropAlt(double delta, int flip){
//   float linDif = tempAlt[flip] - delta;
//   float linPas = tempAlt[flip]
//   Serial.println("Past, pres diff: "+String(linPas)+","+String(linDif));
//   if(linPas*linDif <= 0){
//     tempAlt = 0;
//     return;
//   }else{
//     if (stepAlt==0){
//       tempAlt = delta;
//     }
//     stepAlt = stepAlt+1;
//     return;
//   }
//   Serial.println("SndDropAlt No If");
// }
float globX;
float globY;
float globZ;
void setup() {

  //Initialize Wire
  Wire.begin();
  Wire1.begin();
  Wire2.begin();
  Serial.begin(9600);

  //Intialize Sensors
  while (!Serial);
  Serial.println("Adafruit BMP388 / BMP390 test");
  //Test to see if communicating properly
  if (!bmp.begin_I2C(0x77,&Wire)) {   // hardware I2C mode, can pass in address & alt Wire
  //if (! bmp.begin_SPI(BMP_CS)) {  // hardware SPI mode  
  //if (! bmp.begin_SPI(BMP_CS, BMP_SCK, BMP_MISO, BMP_MOSI)) {  // software SPI mode
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    //while (1);
  }

  Serial.println("ICM20948 Test");
  if (!icm.begin_I2C(0x69,&Wire1)) {

    Serial.println("Failed to find ICM20948 chip");
    delay(10);
  }else{
    Serial.println("ICM20948 Found!");
  }

  Serial.println("ADXL375 Accelerometer Test"); 
  /* Initialize the sensor */
  if(!accel375.begin(0x53))
  {
    /* There was a problem detecting the ADXL375 ... check your connections */
    Serial.println("Ooops, no ADXL375 detected ... Check your wiring!");
  }else{
    Serial.println("ADXL375 Found!");
  }

  //Initialize SD Card
  // Serial.print("Initializing SD card...");
  // if (!SD.begin(BUILTIN_SDCARD)) {
  //   Serial.println("Initialization failed!");
  //   return;
  // }

  Serial.println("Initialization done.");

  //Set IMU Ranges
  icm.setAccelRange(ICM20948_ACCEL_RANGE_16_G);
  icm.setGyroRange(ICM20948_GYRO_RANGE_2000_DPS);
  icm.setMagDataRate(AK09916_MAG_DATARATE_100_HZ);

  //Set up oversampling and filter initialization for BMP390
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);

void sandDropAk(float delta, int flip, int invFlp);
  //void sandDropAlt(float delta);
  
  globX = 0.0;
  globY = 0.0;
  globZ = 0.0;
}


void loop() {
  //Get Altitude Readings
  if (! bmp.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }

  //Get IMU Readings
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t mag;
  sensors_event_t temp;
  icm.getEvent(&accel, &gyro, &temp, &mag);

  //Get Accelerometer Readings
  sensors_event_t event;
  accel375.getEvent(&event);

  //decision switch
  bool grnLight = false;

  //Store Data
  // 1 sec = 1000 milsec
  //long pasT = currT;
  currT = millis();
  //int sec = getSeconds(currT);
  static int cyCow = -1;
  cyCow = cyCow + 1;
  Serial.println("\ncyCow: "+String(cyCow));
  int flip = (cyCow % 2);
  int invFlp = ((flip - 1)*(-1));
  Serial.println("Flip: "+String(flip));
  Serial.println("invFlip: "+String(invFlp));

  //Altitude/Temperature Data
  double bmpTemp = bmp.temperature;
  double bmpPresh = bmp.pressure/100;
  double bmpAlt = bmp.readAltitude(SEALEVELPRESSURE_HPA); 

  //IMU Data
  float imuAccelX = (accel.acceleration.x * -1);
  float imuAccelY = accel.acceleration.y;
  float imuAccelZ = accel.acceleration.z;
  float gyroX = gyro.gyro.x;
  float gyroY = gyro.gyro.y;
  float gyroZ = gyro.gyro.z;

  //Acceleration Data
  float accelX = event.acceleration.x;
  float accelY = event.acceleration.y; 
  float accelZ = event.acceleration.z;  

  //operations
  sandDropAk(imuAccelY, flip, invFlp);
  Serial.println("First cell: "+String(tempAk[0]));
  Serial.println("Second cell: "+String(tempAk[1]));
  tempAk[flip] = imuAccelY;
  //tempAk = imuAccelY;
  // linPas = bmpAlt - tempAlt;
  // sandDropAlt(bmpAlt, linPas);
  // tempAlt = bmpAlt;

  globX += gyroX;
  globY += gyroY;
  globZ += gyroZ;

  // if(aligned){
  //   ();
  // }

  //File dataFile = SD.open("data_Combined.csv", FILE_WRITE);

    //Serial.println("Writing to data.csv...");
    // Create a string to hold the complete output
    String output = "";

    // Print time
    output += "Time: " + String(currT);

    // Print sensor values in the same line
    output += ", BMP Temp: " + String(bmpTemp) + ", BMP Pressure: " + String(bmpPresh) + ", BMP Altitude: " + String(bmpAlt);
    output += ", IMU Accel X Y Z: " + String(imuAccelX) + "," + String(imuAccelY) + "," + String(imuAccelZ);
    output += ", Global X Y Z: " + String(globX) + "," + String(globY) + "," + String(globZ);
    output += ", Gyro Dlt X Y Z: " + String(gyroX) + "," + String(gyroY) + "," + String(gyroZ);
    output += ", Accelo X Y Z: " + String(accelX) + "," + String(accelY) + "," + String(accelZ);
    //output += ", LinDlta Loop Pressure: " + String(stepAlt);
    output += ", LinDlta Loop Y Accel: " + String(stepAk);
    //output += "\n";

    // Write to Serial
    Serial.print(output);

    // Write to file
    //dataFile.println(output);
    //Serial.println("\nData written successfully.");

  //dataFile.close();
delay(180);
}
