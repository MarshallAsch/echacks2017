/*
 * Copyright (c) 2016 Intel Corporation.  All rights reserved.
 * See the bottom of this file for the license terms.
 */

#include <CurieBLE.h>

#include "rgb_lcd.h"


const int BUTTON = 5;
const int STATUS_LED = 2;
const int ZAP = 3;
const int SOUND = 4;
const int ZAP_LIGHT = 6;

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;


rgb_lcd lcd;




BLEPeripheral blePeripheral; // create peripheral instance

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // create service

// create switch characteristic and allow remote device to read and write
BLECharCharacteristic switchChar("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

void setup() {
  Serial.begin(9600);
  
  //set teh override button
  pinMode(BUTTON, INPUT); 

  //set the outputs
  pinMode(STATUS_LED, OUTPUT);
  pinMode(ZAP, OUTPUT);
  pinMode(SOUND, OUTPUT);
  pinMode(ZAP_LIGHT, OUTPUT);

  digitalWrite(STATUS_LED, LOW);
  digitalWrite(ZAP, LOW);
  digitalWrite(SOUND, LOW);
  digitalWrite(ZAP_LIGHT, LOW);

  lcd.begin(16, 2);
    
    lcd.setRGB(colorR, colorG, colorB);
    
    // Print a message to the LCD.
    lcd.print("Not connected...");

  // set the local name peripheral advertises
  blePeripheral.setLocalName("DOG Collar");
  // set the UUID for the service this peripheral advertises
  blePeripheral.setAdvertisedServiceUuid(ledService.uuid());

  // add service and characteristic
  blePeripheral.addAttribute(ledService);
  blePeripheral.addAttribute(switchChar);

  // assign event handlers for connected, disconnected to peripheral
  blePeripheral.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  blePeripheral.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  // assign event handlers for characteristic
  switchChar.setEventHandler(BLEWritten, switchCharacteristicWritten);
  
  // set an initial value for the characteristic
  switchChar.setValue(0);

  // advertise the service
  blePeripheral.begin();
}

void loop() {
  // poll peripheral
  //blePeripheral.poll();

  int btn = digitalRead(BUTTON); //read the status of the button
    digitalWrite(STATUS_LED, btn);
    delay(2);
}

//handler on connect
void blePeripheralConnectHandler(BLECentral& central) {

    lcd.setRGB(0, 255, 0);

    lcd.clear();
    lcd.print("Connected...");
    digitalWrite(STATUS_LED, 1);
    delay(1000);
    digitalWrite(STATUS_LED, 0);
    delay(1000);
    digitalWrite(STATUS_LED, 1);
    delay(1000);
    digitalWrite(STATUS_LED, 0);
    delay(1000);
    digitalWrite(STATUS_LED, 1);
    delay(1000);
    digitalWrite(STATUS_LED, 0);
    delay(1000);

}

//handler on disconnect 
void blePeripheralDisconnectHandler(BLECentral& central) {

lcd.setRGB(255, 0, 0);
lcd.clear();
lcd.print("Not connected...");
    digitalWrite(ZAP, LOW);
    digitalWrite(SOUND, LOW);
    digitalWrite(ZAP_LIGHT, LOW);
  
    digitalWrite(STATUS_LED, HIGH);
    delay(1000);
    digitalWrite(STATUS_LED, LOW);
    delay(1000);
    digitalWrite(STATUS_LED, HIGH);
    delay(1000);
    digitalWrite(STATUS_LED, LOW);
    delay(1000);
    digitalWrite(STATUS_LED, HIGH);
    delay(1000);
    digitalWrite(STATUS_LED, LOW);
    delay(1000);
}

void switchCharacteristicWritten(BLECentral& central, BLECharacteristic& characteristic) {

  
  switch (switchChar.value())
  {
    case 0x01:          //start zap
      digitalWrite(ZAP, HIGH);
      digitalWrite(ZAP_LIGHT, HIGH);
    delay(250);

      digitalWrite(ZAP, LOW);
      digitalWrite(ZAP_LIGHT, LOW);
delay(250);
      digitalWrite(ZAP, HIGH);
      digitalWrite(ZAP_LIGHT, HIGH);
    delay(250);

      digitalWrite(ZAP, LOW);
      digitalWrite(ZAP_LIGHT, LOW);
      break;
    case 0x02:          //end zap
      digitalWrite(ZAP, LOW);
      digitalWrite(ZAP_LIGHT, LOW);
      break;
    case 0x03:          //start sound
      digitalWrite(SOUND, HIGH);
      delay(500);

      digitalWrite(SOUND, LOW);
      digitalWrite(SOUND, LOW);
      break;
    case 0x04:          //end sound
      digitalWrite(SOUND, LOW);
      break;
    default:
      break;
  }
}

