#ifndef HW_UTILS_H
#define HW_UTILS_H

#define SERIAL_TIMEOUT_DURATION 100

#include "main.h"

void controlLed(int ledPin, int duration, bool shouldBlink = false, int blinkInterval = 500);
void asyncLedLoop();
float getHumidity(unsigned long timeoutDuration = SERIAL_TIMEOUT_DURATION);
float getTemperature(unsigned long timeoutDuration = SERIAL_TIMEOUT_DURATION);

#endif
