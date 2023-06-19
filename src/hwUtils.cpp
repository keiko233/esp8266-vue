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

void ledLoop() {
  controlLed(LED_R, findDatabase("led_red_bright").toInt());
  controlLed(LED_R, findDatabase("led_red_blink").toInt(), true);
  controlLed(LED_G, findDatabase("led_green_bright").toInt());
  controlLed(LED_G, findDatabase("led_green_blink").toInt(), true);
  controlLed(LED_B, findDatabase("led_blue_bright").toInt());
  controlLed(LED_B, findDatabase("led_blue_blink").toInt(), true);
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

int getPhotoresistance(unsigned long timeoutDuration) {
  unsigned long startTime = millis();

  while (millis() - startTime < timeoutDuration) {
    if (Serial.available() > 0) {
      String receivedData = Serial.readStringUntil('\n');
      if (receivedData.startsWith("Photoresistance: ")) {
        receivedData.remove(0, 17);
        return receivedData.toInt();
      }
    }
  }

  return 0;
}

int getButtonClick(int buttonNumber, unsigned long timeoutDuration) {
  unsigned long startTime = millis();
  String buttonPrefix = "BTN_" + String(buttonNumber) + ": ";

  while (millis() - startTime < timeoutDuration) {
    if (Serial.available() > 0) {
      String receivedData = Serial.readStringUntil('\n');
      if (receivedData.startsWith(buttonPrefix)) {
        receivedData.remove(0, 7);
        return receivedData.toInt();
      }
    }
  }

  return 0;
}
