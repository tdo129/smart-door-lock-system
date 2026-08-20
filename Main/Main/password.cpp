#include "password.h"
#include "fingerprint.h"
#include <EEPROM.h>

// ================= GLOBAL =================
bool adminMode = false;
static bool adminScreenDrawn = false;

const int EEPROM_SIZE = 64;
const int PASS_ADDR = 0;

String adminPass = "369";

// ======= CHỐNG DỘI PHÍM =======
unsigned long lastKeyTime = 0;
const int debounceDelay = 180;

bool isKeyValid(char k) {
    if (!k) return false;

    if (millis() - lastKeyTime < debounceDelay)
        return false;

    lastKeyTime = millis();
    return true;
}

// =============== EEPROM SAVE/LOAD ============
void savePassword(String pass) {
    EEPROM.writeString(PASS_ADDR, pass);
    EEPROM.commit();
}

String loadPassword() {
    String p = EEPROM.readString(PASS_ADDR);

    if (p.length() < 1)
        return "9999";

    return p;
}

// =============== SETUP =======================
void setupPassword() {

    EEPROM.begin(EEPROM_SIZE);

    password = loadPassword();

    lcd.backlight();
    lcd.clear();
    lcd.print("System Ready");

    beepOK();

    softDelay(600);

    drawIdleScreen();
}

// =============== ADMIN MENU ==================
void showAdminMenu() {

    lcd.clear();

    lcd.print("1:Add 2:Del");

    lcd.setCursor(0,1);

    lcd.print("0:Exit");

    adminScreenDrawn = true;
}

// =============== MAIN LOOP ===================
void loopPassword() {

    keypad.getKeys();

    for (int i = 0; i < LIST_MAX; i++) {

        if (keypad.key[i].kstate != PRESSED)
            continue;

        char key = keypad.key[i].kchar;

        if (!isKeyValid(key))
            continue;

        lcd.backlight();

        // =======================================
        // =============== IDLE ==================
        // =======================================
        if (!enteringPassword && !adminMode) {

            if (key == '#') {

                enteringPassword = true;

                inputPass = "";

                lcd.clear();

                lcd.print("Enter Password");

                lcd.setCursor(0,1);

                lcd.print("----");
            }

            return;
        }

        // =======================================
        // ========== PASSWORD MODE ==============
        // =======================================
        if (enteringPassword) {

            // ---- Thoát ----
            if (key == '*') {

                enteringPassword = false;

                inputPass = "";

                drawIdleScreen();

                return;
            }

            // ---- Nhập số ----
            if (key >= '0' && key <= '9') {

                if (inputPass.length() < 32)
                    inputPass += key;

                lcd.setCursor(0,1);
                lcd.print("                ");

                lcd.setCursor(0,1);

                printStars(min((int)inputPass.length(), 16));

                // ===== USER PASSWORD =====
                if (inputPass == password) {

                    lcd.clear();
                    lcd.print("Password OK");

                    beepOK();

                    openDoor();

                    inputPass = "";

                    enteringPassword = false;

                    softDelay(300);

                    drawIdleScreen();

                    return;
                }

                // ===== ADMIN PASSWORD =====
                if (inputPass == adminPass) {

                    inputPass = "";

                    enteringPassword = false;

                    adminMode = true;

                    adminScreenDrawn = false;

                    showAdminMenu();

                    return;
                }

                // ===== WRONG PASSWORD =====
                if (inputPass.length() >= password.length() &&
                    inputPass != password &&
                    inputPass != adminPass) {

                    lcd.clear();
                    lcd.print("Wrong Password");

                    // Nếu có hàm báo lỗi thì bỏ comment dòng dưới
                    // beepError();

                    softDelay(1500);

                    inputPass = "";

                    enteringPassword = false;

                    drawIdleScreen();

                    return;
                }
            }

            return;
        }

        // =======================================
        // ============== ADMIN ==================
        // =======================================
        if (adminMode) {

            if (!adminScreenDrawn)
                showAdminMenu();

            // ---- Exit ----
            if (key == '0') {

                adminMode = false;

                adminScreenDrawn = false;

                drawIdleScreen();

                return;
            }

            // ---- Add Finger ----
            if (key == '1') {

                lcd.clear();

                lcd.print("Add Finger");

                addFingerprint();

                adminMode = false;

                adminScreenDrawn = false;

                drawIdleScreen();

                return;
            }

            // ---- Delete Finger ----
            if (key == '2') {

                lcd.clear();

                lcd.print("Delete Finger");

                deleteFingerprint();

                adminMode = false;

                adminScreenDrawn = false;

                drawIdleScreen();

                return;
            }
        }
    }
}

// =============== IDLE SCREEN =================
void drawIdleScreen() {

    lcd.backlight();

    lcd.clear();

    lcd.print("Ready");

    lcd.setCursor(0,1);

    lcd.print("#:Unlock");
}