#ifndef HW_UTILS_H
#define HW_UTILS_H

#include "main.h"

void controlLed(int ledPin, int duration, bool shouldBlink = false, int blinkInterval = 500);
void asyncLedLoop();
float getHumidity(unsigned long timeoutDuration = strtoul(findDatabase("serial_timeout_duration").c_str(), nullptr, 10));
float getTemperature(unsigned long timeoutDuration = strtoul(findDatabase("serial_timeout_duration").c_str(), nullptr, 10));
int getPhotoresistance(unsigned long timeoutDuration = strtoul(findDatabase("serial_timeout_duration").c_str(), nullptr, 10));
int getButtonClick(int buttonNumber, unsigned long timeoutDuration = strtoul(findDatabase("serial_timeout_duration").c_str(), nullptr, 10));

#endif
