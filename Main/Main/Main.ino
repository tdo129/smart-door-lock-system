#include "hardware.h"
#include "fingerprint.h"
#include "password.h"

// ================= FLAGS =================
bool addFpMode      = false;
bool changePassMode = false;
bool deleteFpMode   = false;
bool relayManual    = false;

// ================= STATE =================
bool doorState  = false;
bool lightState = false;

// ================= FINGERPRINT ===========
int fingerprintCount  = 0;
int lastFingerprintID = -1;

void setup() {
  Serial.begin(115200);

  setupHardware();
  setupFingerprint();
  setupPassword();

  lcd.init();
  lcd.backlight();
  lcd.clear();

  fingerprintCount = getFingerprintCount();

  lcd.clear();
  lcd.print("System Ready");
}

void loop() {
  // Quét mật khẩu từ keypad
  loopPassword();

  // Quét vân tay
  loopFingerprint();

  // Đèn tự động bằng radar nếu không bật thủ công
  if (!relayManual) {
    handleRadar();
    lightState = digitalRead(RELAY);
  }

  // Thêm vân tay
  if (addFpMode) {
    lcd.clear();
    lcd.print("Add Finger...");

    addFingerprint();
    delay(300);

    fingerprintCount = getFingerprintCount();
    addFpMode = false;

    lcd.clear();
    lcd.print("System Ready");
  }

  // Xóa vân tay
  if (deleteFpMode) {
    lcd.clear();
    lcd.print("Delete Finger");

    deleteFingerprint();
    delay(300);

    fingerprintCount = getFingerprintCount();
    deleteFpMode = false;

    lcd.clear();
    lcd.print("System Ready");
  }

  // Đổi mật khẩu
  if (changePassMode) {
    lcd.clear();
    lcd.print("New Password:");

    inputPass = "";
    enteringPassword = true;
    changePassMode = false;
  }
}