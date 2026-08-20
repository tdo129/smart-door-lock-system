#ifndef HARDWARE_H
#define HARDWARE_H

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HardwareSerial.h>
#include <ESP32Servo.h>
#include <Keypad.h>
#include <EEPROM.h>
#include "Adafruit_Fingerprint.h"

// ======= EXTERN BIẾN =======
extern LiquidCrystal_I2C lcd;
extern HardwareSerial mySerial;
extern Adafruit_Fingerprint finger;
extern Servo myservo;

extern bool enteringPassword;
extern String inputPass;
extern String password;

extern bool doorOpen;

// Keypad
extern Keypad keypad;

// Pin
extern const int BUZZER;
extern const int RELAY;
extern const int SERVO_PIN;
extern const int RADAR_OUT;

// ======= HÀM =======
void setupHardware();

void beepOK();
void beepError();

void openDoor();
void printStars(int n);

void handleRadar();
void softDelay(unsigned long ms);

#endif