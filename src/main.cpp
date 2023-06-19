#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

const int DHTPIN = 2;
const int PHOTORESISTANCE = A0;

DHT dht(DHTPIN, DHT11);

void setup() {
  Serial.begin(115200);
  pinMode(PHOTORESISTANCE, INPUT);
  dht.begin();
}

void loop() {
  Serial.print("Humidity: ");
  Serial.println(dht.readHumidity());

  Serial.print("Temperature: ");
  Serial.println(dht.readTemperature());

  Serial.print("Photoresistance: ");
  Serial.println(analogRead(PHOTORESISTANCE));
}
