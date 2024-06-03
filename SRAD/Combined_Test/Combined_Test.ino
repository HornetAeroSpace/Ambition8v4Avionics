#include <SPI.h>
//#include <LoRa.h>
#include "Arduino.h"
#include <Wire.h>
#include <string>
#include <SD.h> 
#include <RTClib.h>
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
  Serial.print("Initializing SD card...");
  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("Initialization failed!");
    return;
  }

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

  //Set Accelerometer data rate
  
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

  //Store Data
  float sensorValues[14];   
  int t = millis();
  //Serial.println(t);
  //Serial.println(getMinutes(t));
  //Serial.println(getSeconds(t));
  //Time Data Retrieved
  sensorValues[0] = getMinutes(t); 
  sensorValues[1] = getSeconds(t); 

  //Altitude/Temperature Data
  sensorValues[2] = bmp.temperature;  
  sensorValues[3] = bmp.pressure/100; 
  sensorValues[4] = bmp.readAltitude(SEALEVELPRESSURE_HPA); 

  //IMU Data
  sensorValues[5] = accel.acceleration.x;  
  sensorValues[6] = accel.acceleration.y; 
  sensorValues[7] = accel.acceleration.z;
  sensorValues[8] = gyro.gyro.x; 
  sensorValues[9] = gyro.gyro.y; 
  sensorValues[10] = gyro.gyro.z; 

  //Acceleration Data
  sensorValues[11] = event.acceleration.x;  
  sensorValues[12] = event.acceleration.y; 
  sensorValues[13] = event.acceleration.z; 

  File dataFile = SD.open("data_Combined.csv", FILE_WRITE);

  if (dataFile) {
    Serial.println("Writing to data.csv...");

    // Write the header to the CSV file
    for (int i = 0; i < 14; i++) {
      dataFile.print(",");
      dataFile.print(sensorValues[i]);
    }
    dataFile.println();
    Serial.println("Data written successfully.");
  } else {
    // If the file didn't open, print an error
    Serial.println("Error opening data.csv");
  }

  dataFile.close();

}

float getSeconds(long time){
  return float(abs((time/1000)-(round(time/60000)*60)));
}

float getMinutes(long time){
  return float(round(time/60000));
}
