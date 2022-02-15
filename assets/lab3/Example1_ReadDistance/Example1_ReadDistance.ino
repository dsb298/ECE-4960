#include <ComponentObject.h>
#include <RangeSensor.h>
#include <SparkFun_VL53L1X.h>
#include <vl53l1x_class.h>
#include <vl53l1_error_codes.h>

/*
  Reading distance from the laser based VL53L1X
  By: Nathan Seidle
  SparkFun Electronics
  Date: April 4th, 2018
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  SparkFun labored with love to create this code. Feel like supporting open source hardware?
  Buy a board from SparkFun! https://www.sparkfun.com/products/14667

  This example prints the distance to an object.

  Are you getting weird readings? Be sure the vacuum tape has been removed from the sensor.
*/

#include <Wire.h>
#include "SparkFun_VL53L1X.h" //Click here to get the library: http://librarymanager/All#SparkFun_VL53L1X

#define FRONT_SHUTDOWN A1
#define LEFT_SHUTDOWN A2

SFEVL53L1X frontSensor(Wire, FRONT_SHUTDOWN, -1);
SFEVL53L1X leftSensor(Wire, LEFT_SHUTDOWN, -1);

void setup(void)
{
  pinMode(LEFT_SHUTDOWN, OUTPUT);
  pinMode(FRONT_SHUTDOWN, OUTPUT);
  digitalWrite(LEFT_SHUTDOWN, LOW);
  digitalWrite(FRONT_SHUTDOWN, LOW);
  delay(500);
  
  Wire.begin();

  Serial.begin(115200);
  Serial.println("VL53L1X Qwiic Test");

  // FRONT SENSOR INIT
  pinMode(FRONT_SHUTDOWN, INPUT);
  delay(150);
  if (frontSensor.begin() != 0)
  {
    Serial.println("Front sensor failed to initialize!");
    while (1)
      ;
  }
  delay(100);
  frontSensor.setI2CAddress((uint8_t)0x54);


  // LEFT SENSOR INIT
  pinMode(LEFT_SHUTDOWN, INPUT);
  delay(150);
  if (leftSensor.begin() != 0)
  {
    Serial.println("Left sensor failed to initialize!");
    while (1)
      ;
  }
  delay(100);
  
  Serial.println("Sensors online!");

  Serial.print("0x");
  Serial.println(frontSensor.getI2CAddress(), HEX);
  Serial.print("0x");
  Serial.println(leftSensor.getI2CAddress(), HEX);
}

void loop(void)
{
    // Front Sensor
  frontSensor.startRanging();

  while(!frontSensor.checkForDataReady())
  {
    delay(1);
  }
  int frontDistance = frontSensor.getDistance();
  
  frontSensor.clearInterrupt();
  frontSensor.stopRanging();

  
  // Left Sensor
  leftSensor.startRanging();

  while(!leftSensor.checkForDataReady())
  {
    delay(1);
  }
  int leftDistance = leftSensor.getDistance();
  
  leftSensor.clearInterrupt();
  leftSensor.stopRanging();


  // Printout
  Serial.println("*************************************");
  Serial.print("Front Distance(mm): ");
  Serial.println(frontDistance);
  Serial.print("Left Distance(mm): ");
  Serial.println(leftDistance);
  delay(1000);
}
