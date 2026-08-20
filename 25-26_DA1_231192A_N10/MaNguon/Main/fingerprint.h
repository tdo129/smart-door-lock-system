#ifndef FINGERPRINT_H
#define FINGERPRINT_H

#include "hardware.h"

void setupFingerprint();
void loopFingerprint();

void addFingerprint();
void deleteFingerprint();

int getIDFromKeypad();
int getFingerprintCount();

#endif