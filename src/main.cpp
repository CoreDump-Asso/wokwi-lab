#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// Never use gpio 6~11 -> internal flash
// gpio 34~39 -> input only, MISO can use them
// CS selects the chip to talk to

#define SCK 18
#define MISO 19
#define MOSI 23
#define CS 27

Adafruit_BME280 bme(CS);

void setup() {
  Serial.begin(115200);

  // SPI ESP32 explicite (IMPORTANT)
  SPI.begin(SCK, MISO, MOSI, CS);

  // Petit délai stabilité Wokwi
  delay(100);

  if (!bme.begin(0x76)) {   // IMPORTANT: parfois requis même en SPI Wokwi
    Serial.println("Erreur BME280 SPI");
    while (1);
  }

  Serial.println("BME280 OK");
}

void loop() {
  Serial.print("Temp: ");
  Serial.println(bme.readTemperature());

  Serial.print("Hum: ");
  Serial.println(bme.readHumidity());

  Serial.print("Press: ");
  Serial.println(bme.readPressure() / 100.0F);

  delay(2000);
}