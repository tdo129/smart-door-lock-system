#ifndef PASSWORD_H
#define PASSWORD_H

#include "hardware.h"

void setupPassword();
void loopPassword();

void drawIdleScreen();

String loadPassword();
void savePassword(String pass);

#endif