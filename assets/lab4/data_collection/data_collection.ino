#include <ComponentObject.h>
#include <RangeSensor.h>
#include <SparkFun_VL53L1X.h>
#include <vl53l1x_class.h>
#include <vl53l1_error_codes.h>

#include "BLECStringCharacteristic.h"
#include "EString.h"
#include "RobotCommand.h"
#include <ArduinoBLE.h>

//////////// BLE UUIDs ////////////
#define BLE_UUID_TEST_SERVICE "9A48ECBA-2E92-082F-C079-9E75AAE428B1"

#define BLE_UUID_RX_STRING "9750f60b-9c9c-4158-b620-02ec9521cd99"

#define BLE_UUID_TX_FLOAT "27616294-3063-4ecc-b60b-3470ddef2938"
#define BLE_UUID_TX_STRING "f235a225-6735-4d73-94cb-ee5dfce9ba83"
//////////// BLE UUIDs ////////////

//////////// Global Variables ////////////
BLEService testService(BLE_UUID_TEST_SERVICE);

BLECStringCharacteristic rx_characteristic_string(BLE_UUID_RX_STRING, BLEWrite, MAX_MSG_SIZE);

BLEFloatCharacteristic tx_characteristic_float(BLE_UUID_TX_FLOAT, BLERead | BLENotify);
BLECStringCharacteristic tx_characteristic_string(BLE_UUID_TX_STRING, BLERead | BLENotify, MAX_MSG_SIZE);

// RX
RobotCommand robot_cmd(":|");

// TX
EString tx_estring_value;
float tx_float_value = 0.0;

long interval = 500;
static long previousMillis = 0;
unsigned long currentMillis = 0;
//////////// Global Variables ////////////

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
  

  BLE.begin();
  // Set advertised local name and service
  BLE.setDeviceName("Artemis BLE");
  BLE.setLocalName("Artemis BLE");
  BLE.setAdvertisedService(testService);

  // Add BLE characteristics
  testService.addCharacteristic(tx_characteristic_float);
  testService.addCharacteristic(tx_characteristic_string);
  testService.addCharacteristic(rx_characteristic_string);

  // Add BLE service
  BLE.addService(testService);

  // Initial values for characteristics
  // Set initial values to prevent errors when reading for the first time on central devices
  tx_characteristic_float.writeValue(0.0);

  /*
   * An example using the EString
   */
  // Clear the contents of the EString before using it
  tx_estring_value.clear();

  // Append the string literal "[->"
  tx_estring_value.append("[->");

  // Append the float value
  tx_estring_value.append(9.0);

  // Append the string literal "<-]"
  tx_estring_value.append("<-]");

  // Write the value to the characteristic
  tx_characteristic_string.writeValue(tx_estring_value.c_str());

  // Output MAC Address
  Serial.print("Advertising BLE with MAC: ");
  Serial.println(BLE.address());

  BLE.advertise();
}

int frontDistance;

void readSensor()
{
  // Front Sensor
  frontSensor.startRanging();

  while(!frontSensor.checkForDataReady())
  {
    delay(1);
  }
  frontDistance = frontSensor.getDistance();
  
  frontSensor.clearInterrupt();
  frontSensor.stopRanging();
}

void loop(void)
{
  int a = millis();

  // bluetooth
  // Listen for connections
  BLEDevice central = BLE.central();

  // If a central is connected to the peripheral
  if (central) {
    Serial.print("Connected to: ");
    Serial.println(central.address());

    // While central is connected
    while (central.connected()) {
      int a = millis();

      // get data
      char char_arr[] = itos(frontDistance);
      strcat(char_arr, "\n");
      delay(25);
      char elTime[] = itoa(millis()-a);
      strcat(char_arr, elTime);
      
      tx_estring_value.clear();
      tx_estring_value.append(char_arr);
      tx_characteristic_string.writeValue(tx_estring_value.c_str());
    }

    Serial.println("Disconnected");
  }
}
