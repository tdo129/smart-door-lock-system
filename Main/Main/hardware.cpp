#include "hardware.h"

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Fingerprint
HardwareSerial mySerial(2);
Adafruit_Fingerprint finger(&mySerial);

// Servo
Servo myservo;

// Password
bool enteringPassword = false;
String inputPass = "";
String password = "";

// Door state
bool doorOpen = false;

// Pins
const int BUZZER    = 2;
const int RELAY     = 4;
const int SERVO_PIN = 23;
const int RADAR_OUT = 5;

// Keypad
const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {32, 33, 25, 26};
byte colPins[COLS] = {27, 14, 12, 13};

Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void beepOK() {
  digitalWrite(BUZZER, HIGH);
  delay(100);
  digitalWrite(BUZZER, LOW);
}

void beepError() {
  for (int i = 0; i < 2; i++) {
    digitalWrite(BUZZER, HIGH);
    delay(80);
    digitalWrite(BUZZER, LOW);
    delay(80);
  }
}

void printStars(int n) {
  for (int i = 0; i < n; i++) {
    lcd.print("*");
  }
}

void openDoor() {
  doorOpen = true;

  beepOK();

  lcd.clear();
  lcd.print("Door Open");

  myservo.write(90);
  delay(3000);

  myservo.write(0);
  delay(1200);

  doorOpen = false;

  lcd.clear();
  lcd.print("Door Closed");
  delay(800);

  lcd.clear();
  lcd.print("System Ready");
}

void handleRadar() {
  int state = digitalRead(RADAR_OUT);

  if (state == HIGH) {
    digitalWrite(RELAY, HIGH);
  } else {
    digitalWrite(RELAY, LOW);
  }
}

void setupHardware() {
  Serial.begin(115200);
  EEPROM.begin(64);

  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);

  pinMode(RADAR_OUT, INPUT);

  myservo.attach(SERVO_PIN);
  myservo.write(0);

  Wire.begin(18, 19);

  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void softDelay(unsigned long ms) {
  unsigned long start = millis();

  while (millis() - start < ms) {
    keypad.getKeys();
    delay(1);
  }
}