#include "fingerprint.h"

void setupFingerprint() {
  mySerial.begin(57600, SERIAL_8N1, 21, 22);
  finger.begin(57600);

  lcd.clear();
  lcd.print("Check Finger...");

  if (finger.verifyPassword()) {
    lcd.clear();
    lcd.print("Finger OK");
  } else {
    lcd.clear();
    lcd.print("Finger Error");
  }

  softDelay(1000);
  lcd.clear();
}

void loopFingerprint() {
  if (enteringPassword) return;

  uint8_t p = finger.getImage();

  if (p == FINGERPRINT_NOFINGER) {
    return;
  }

  lcd.backlight();
  lcd.clear();
  lcd.print("Processing...");

  if (p != FINGERPRINT_OK) {
    return;
  }

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) {
    beepError();
    lcd.clear();
    lcd.print("Img Error");
    softDelay(800);
    lcd.clear();
    return;
  }

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) {
    beepError();
    lcd.clear();
    lcd.print("Unknown Finger");
    softDelay(1000);
    lcd.clear();
    return;
  }

  lcd.clear();
  lcd.print("ID: ");
  lcd.print(finger.fingerID);
  lcd.setCursor(0, 1);
  lcd.print("Access OK");

  beepOK();

  openDoor();

  lcd.clear();
  lcd.print("System Ready");
}

void addFingerprint() {
  int id = getIDFromKeypad();
  if (id < 0) return;

  lcd.clear();
  lcd.print("Place Finger");

  while (finger.getImage() != FINGERPRINT_OK) {
    keypad.getKeys();

    char key = keypad.getKey();
    if (key == '*') {
      lcd.clear();
      lcd.print("Cancel");
      softDelay(800);
      lcd.clear();
      return;
    }

    softDelay(10);
  }

  if (finger.image2Tz(1) != FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("Img Error");
    beepError();
    softDelay(1000);
    return;
  }

  lcd.clear();
  lcd.print("Remove Finger");
  softDelay(1500);

  lcd.clear();
  lcd.print("Place Again");

  while (finger.getImage() != FINGERPRINT_OK) {
    keypad.getKeys();

    char key = keypad.getKey();
    if (key == '*') {
      lcd.clear();
      lcd.print("Cancel");
      softDelay(800);
      lcd.clear();
      return;
    }

    softDelay(10);
  }

  if (finger.image2Tz(2) != FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("Img Error");
    beepError();
    softDelay(1000);
    return;
  }

  if (finger.createModel() != FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("Not Match");
    beepError();
    softDelay(1500);
    return;
  }

  if (finger.storeModel(id) == FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("Added ID ");
    lcd.print(id);
    beepOK();
    softDelay(1500);
  } else {
    lcd.clear();
    lcd.print("Store Fail");
    beepError();
    softDelay(1500);
  }

  lcd.clear();
  lcd.print("System Ready");
}

void deleteFingerprint() {
  int id = getIDFromKeypad();
  if (id < 0) return;

  if (finger.deleteModel(id) == FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("Deleted ID ");
    lcd.print(id);
    beepOK();
    softDelay(1500);
  } else {
    lcd.clear();
    lcd.print("Delete Fail");
    beepError();
    softDelay(1500);
  }

  lcd.clear();
  lcd.print("System Ready");
}

int getIDFromKeypad() {
  lcd.clear();
  lcd.print("Enter ID:");
  lcd.setCursor(0, 1);

  String idStr = "";

  while (true) {
    keypad.getKeys();

    for (int i = 0; i < LIST_MAX; i++) {
      if (keypad.key[i].stateChanged && keypad.key[i].kstate == PRESSED) {
        char key = keypad.key[i].kchar;

        if (key == '*') {
          lcd.clear();
          lcd.print("Cancel");
          softDelay(800);
          lcd.clear();
          return -1;
        }

        if (key >= '0' && key <= '9') {
          if (idStr.length() < 3) {
            idStr += key;
            lcd.print(key);
          }
        }

        if (key == '#') {
          if (idStr.length() == 0) {
            lcd.clear();
            lcd.print("No ID");
            softDelay(800);
            lcd.clear();
            lcd.print("Enter ID:");
            lcd.setCursor(0, 1);
            continue;
          }

          int id = idStr.toInt();

          if (id < 1 || id > 127) {
            lcd.clear();
            lcd.print("ID 1-127 only");
            softDelay(1000);

            lcd.clear();
            lcd.print("Enter ID:");
            lcd.setCursor(0, 1);

            idStr = "";
          } else {
            return id;
          }
        }

        softDelay(180);
      }
    }
  }
}

int getFingerprintCount() {
  finger.getTemplateCount();
  return finger.templateCount;
}