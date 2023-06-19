#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

const int DHTPIN = 2;
const int PHOTORESISTANCE = A0;
const int BTN_4 = 4;
const int BTN_7 = 7;

int BTN_4_CLICK = 0;
int BTN_7_CLICK = 0;

int BTN_4_STATE = HIGH;
int BTN_4_LAST_STATE = HIGH;

int BTN_7_STATE = HIGH;
int BTN_7_LAST_STATE = HIGH;

DHT dht(DHTPIN, DHT11);

void setup() {
  Serial.begin(115200);
  pinMode(PHOTORESISTANCE, INPUT);
  pinMode(BTN_4, INPUT);
  pinMode(BTN_7, INPUT);
  dht.begin();
}

void loop() {
  Serial.print("Humidity: ");
  Serial.println(dht.readHumidity());

  Serial.print("Temperature: ");
  Serial.println(dht.readTemperature());

  Serial.print("Photoresistance: ");
  Serial.println(analogRead(PHOTORESISTANCE));

  int BTN_4_reading = digitalRead(BTN_4);
  if (BTN_4_reading != BTN_4_LAST_STATE) {
    if (BTN_4_reading == LOW) {
      BTN_4_CLICK++;
    }
  }
  BTN_4_LAST_STATE = BTN_4_reading;

  Serial.print("BTN_4: ");
  Serial.println(BTN_4_CLICK);

  int BTN_7_reading = digitalRead(BTN_7);
  if (BTN_7_reading != BTN_7_LAST_STATE) {
    if (BTN_7_reading == LOW) {
      BTN_7_CLICK++;
    }
  }
  BTN_7_LAST_STATE = BTN_7_reading;

  Serial.print("BTN_7: ");
  Serial.println(BTN_7_CLICK);
}
