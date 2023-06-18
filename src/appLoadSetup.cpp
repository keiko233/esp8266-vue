#include "appLoadSetup.h"

void appLoadSetup(void) {
  String fan_speed = findDatabase("fan_speed");
  int speed = fan_speed.toInt();
  analogWrite(FAN_PWM, speed);
}
