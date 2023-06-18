#ifndef HW_UTILS_H
#define HW_UTILS_H

#include "main.h"

void controlLed(int ledPin, int duration, bool shouldBlink = false, int blinkInterval = 500);
void asyncLedLoop();

#endif
