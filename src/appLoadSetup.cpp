#include "appLoadSetup.h"

void appLoadSetup(void) {
  analogWrite(FAN_PWM, findDatabase("fan_speed").toInt());
}
