
/* 

Index:
1. " ********* " means please check back on this
2. method details in README.txt
---------------------------------------------

 Name:           Ambition Avionics c++ file
 Summary:        Code is written to run on a Teensy 4.1 to preform the functions of an avionics subsytem 
 Contributors:   Bahran T, Miguel V, Andre H, Tarandeep S, Micheal B.		 
 Date:           05/14/2024



 Revisions on 05/14 : 
   set up genera


*/ 


// Import necessary libraries 

#include <SPI.h>
#include <LoRa.h>
#include <vector>
#include "Arduino.h"
#include <Wire.h>
#include <string>

// **** Necessary dependencies to download
#include "Adafruit_ICM20948.h"
#include "Adafruit_ICM20X.h"
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
#include "Adafruit_ADXL375.h"
#include <Wire.h>

//******


using namespace std;

// *****! Define constants (Set up 2 more i^2C busses and one SPI) 

/*
* Constants for lora setup 
#define LORA_BAND 433E6 
#define SCK    system clock 
#define MISO   Master in slave out 
#define MOSI   Master out slave in
#define SS     idk 
#define RST    reset 
#define DIO    interrupt request 


what shoudl 

*/

#define ICM_SDA 17
#define ICM_SCL 16

#define BMP_SDA 18
#define BMP_SCL 19

#define ACC_SDA 25
#define ACC_SCL 24


// Funciton prototypes 

bool turnOn();        // "Turns on" flight computer
bool apogeeReached(); // returns T/F based on wether the rocket has reached apogee or not 
string identify();    // finds and identifies problems 
bool Await();         // may be 
void admin();         // menu for doing shit with the rocket: calibrate sensors,see battery life, check RSSI etc.. 
void deployCharges(); // NEEDS WORK, LOGIC NOT YET DETERMINED 

void inFlight();      // writes writes to sdCard and sends packets to groudstation

void recordMessage(); // writes to sdCard and sends packets to groudstatoin




// Global variables 

vector<string[]> errorCodes;

Adafruit_BMP3XX bmp;   // barometric pressure sensor
Adafruit_ICM20948 icm; // Inertial measurment unit sensor 
Adafruit_ADXL375 acc   // Accelerometer

wire.begin();
wire1.begin();
wire2.begin();


void setup() {

//----- LoRa Setup: initalize ports, set transmission bandwidth in b/s 

	Serial.begin(5000000); 

	LoRa.setPins(SS, RST, DIO); 

	LoRa.begin(LORA_BAND); 
//------------------------------------------------









}



void loop() {

	if (turnOn()/*replace with await method... */ ) {

		errorCodes.pushBack(Identify()); 




	}


}




//----------- Function Definitons ----------------------------

bool turnOn() {

	// set up handshake mechanism with groundstation 

	if (Await()) {    // if returns true, exec

	}


}

bool apogeeReached() {


}
string identify() {

	string[] errors = {"low battery","low RSSI","General BMP error ","general accelerometer error", "general IMU error"}

	if (/*check if x is wrong */) { error[0] = ""; }
	if () { error[1] = ""; }
	if () { error[2] = ""; }
	if () { error[3] = ""; }
	if () { error[4] = ""; }

	return errors[]; 

}

bool await(){

	//Method awaits on groundstation packet 

	string target = "whatsThePassword?"; 

	string message = "";

	while (message!=target) {

		message = LoRa.parsePacket();

			if (message = target) {
				return true; 
			}
	}

	return false; 

}

void admin() {



}

void deployCharges() {


}


//---------  MAINTANCE METHODS  --------------------

/*
* Methods to add: 
* transmit method
* promiscuous mode method
* 
* 
* 
* Notes: 
* 1. dont forget to write an index at the top i.g. Lines 23 find() is defined, etc..
* 2. adjust coding rate when RSSI drops
* 
* 2. low battery, signal strength indicator, 
*/