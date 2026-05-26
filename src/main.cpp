#include "DHTesp.h"

const int DHT_PIN = 15;

const int HOT_LED = 25;
const int CLD_LED = 26;
const int DRY_LED = 27;

const float THRESHOLD_COLD = 19.0f;
const float THRESHOLD_HOT  = 25.0f;
const float THRESHOLD_HUM  = 50.0f;

const float MAX_FAN_SPEED = 5.0f;

DHTesp dhtSensor;

enum ModeFlags {
  HOT  = 1 << 0,
  COLD = 1 << 1,
  DRY  = 1 << 2
};

void modeAutoSelect(TempAndHumidity &data, uint8_t &mode) {
  if (data.temperature > THRESHOLD_HOT) {
    mode |= COLD;
  }

  if (data.temperature < THRESHOLD_COLD) {
    mode |= HOT;
  }

  if (data.humidity > THRESHOLD_HUM) {
    mode |= DRY;
  }
}

void printMode(uint8_t &mode) {
  Serial.print("Mode: ");

  if (!mode) {
    Serial.println("Off");
    return;
  }

  if (mode & HOT)  Serial.print("Hot ");
  if (mode & COLD) Serial.print("Cold ");
  if (mode & DRY)  Serial.print("Dry ");

  Serial.println();
}

float calculateDiff(TempAndHumidity &data, u_int8_t &mode) {
  float diff =
    (mode & HOT)  ? data.temperature - THRESHOLD_COLD  :
    (mode & COLD) ? data.temperature - THRESHOLD_HOT :
                    0.0f;
  return min(abs(diff), MAX_FAN_SPEED);
}

void setup() {
  Serial.begin(115200);

  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);

  pinMode(HOT_LED, OUTPUT);
  pinMode(CLD_LED, OUTPUT);
  pinMode(DRY_LED, OUTPUT);
}

void loop() {

  TempAndHumidity data = dhtSensor.getTempAndHumidity();

  uint8_t mode = 0;

  modeAutoSelect(data, mode);

  Serial.println("Temp: " + String(data.temperature, 1) + "°C");
  Serial.println("Humidity: " + String(data.humidity, 1) + "%");

  printMode(mode);

  float fanSpeed = calculateDiff(data, mode);

  if (fanSpeed > 0.0f) {
    Serial.println("Fan Speed: " + String(fanSpeed, 0));
  }

  Serial.println("---");

  digitalWrite(HOT_LED, mode & HOT);
  digitalWrite(CLD_LED, mode & COLD);
  digitalWrite(DRY_LED, mode & DRY);

  delay(2000);
}