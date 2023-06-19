#include "hwUtils.h"

void controlLed(int ledPin, int duration, bool shouldBlink, int blinkInterval) {
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_B, LOW);
  digitalWrite(LED_G, LOW);

  if (shouldBlink) {
    int blinkCount = duration / (2 * blinkInterval);
    for (int i = 0; i < blinkCount; i++) {
      digitalWrite(ledPin, LOW);
      delay(blinkInterval);
      digitalWrite(ledPin, HIGH);
      delay(blinkInterval);
    }
  } else {
    digitalWrite(ledPin, HIGH);
    delay(duration);
  }
}

void asyncLedLoop() {
  while (true) {
    controlLed(LED_R, findDatabase("led_red_bright").toInt());
    controlLed(LED_R, findDatabase("led_red_blink").toInt(), true);
    controlLed(LED_G, findDatabase("led_green_bright").toInt());
    controlLed(LED_G, findDatabase("led_green_blink").toInt(), true);
    controlLed(LED_B, findDatabase("led_blue_bright").toInt());
    controlLed(LED_B, findDatabase("led_blue_blink").toInt(), true);
  }
}

float getHumidity(unsigned long timeoutDuration) {
  unsigned long startTime = millis();

  while (millis() - startTime < timeoutDuration) {
    if (Serial.available() > 0) {
      String receivedData = Serial.readStringUntil('\n');
      if (receivedData.startsWith("Humidity: ")) {
        receivedData.remove(0, 10);
        return receivedData.toFloat();
      }
    }
  }

  return 0.0;
}

float getTemperature(unsigned long timeoutDuration) {
  unsigned long startTime = millis();

  while (millis() - startTime < timeoutDuration) {
    if (Serial.available() > 0) {
      String receivedData = Serial.readStringUntil('\n');
      if (receivedData.startsWith("Temperature: ")) {
        receivedData.remove(0, 12);
        return receivedData.toFloat();
      }
    }
  }

  return 0.0;
}
