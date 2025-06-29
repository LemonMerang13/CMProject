#include <Wire.h>
#include <MPU6050.h>
#include "arduinoFFT.h"
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

#define SAMPLES 64
#define SAMPLING_FREQUENCY 100.0  // Hz

double vReal[SAMPLES];
double vImag[SAMPLES];

MPU6050 mpu;
ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, SAMPLES, SAMPLING_FREQUENCY);

// Wi-Fi credentials
const char* ssid = "Mrang";
const char* password = "12345678";

// ThingSpeak credentials
const char* writeAPIKey = "VYT2XSSB4NB7WQJF";
unsigned long channelNumber = 2989950;

WiFiClient client;

void setup() {
  Serial.begin(115200);
  Wire.begin(D2, D1);  // ESP8266 default I2C pins
  delay(100);

  mpu.initialize();
  if (mpu.testConnection()) {
    Serial.println("MPU6050 connected.");
    mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
    mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_250);
  } else {
    Serial.println("MPU6050 connection failed.");
    while (1);
  }

  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");

  unsigned long startTime = millis();
  const unsigned long timeout = 10000;  // 10 seconds

  while (WiFi.status() != WL_CONNECTED && millis() - startTime < timeout) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(" connected.");
    ThingSpeak.begin(client);
  } else {
    Serial.println(" failed to connect.");
  }
}

void loop() {
  unsigned long timer;
  unsigned long sampling_period_us = 1000000 / SAMPLING_FREQUENCY;
  double sumSq = 0.0;

  // 1. Sampling acceleration data (X-axis) in m/s²
  for (int i = 0; i < SAMPLES; i++) {
    timer = micros();

    int16_t ax, ay, az, gx, gy, gz;
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    double ax_ms2 = ((double)ax / 16384.0) * 9.80665;

    vReal[i] = ax_ms2;
    vImag[i] = 0.0;

    sumSq += ax_ms2 * ax_ms2;

    while (micros() - timer < sampling_period_us);
  }

  // 2. Compute RMS value
  double rms = sqrt(sumSq / SAMPLES);

  // 3. Perform FFT for visualization
  FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward);
  FFT.compute(FFTDirection::Forward);
  FFT.complexToMagnitude();

  // 4. Print FFT Spectrum to Serial Plotter (resetting each time)
  double frequencyStep = SAMPLING_FREQUENCY / SAMPLES;

  for (int i = 1; i < SAMPLES / 2; i++) {  // skip DC component
    double freq = i * frequencyStep;
    Serial.print(freq, 1);
    Serial.print("\t");
    Serial.println(vReal[i], 3);
  }

  // 5. Print RMS value to Serial Monitor
  Serial.print("RMS value: ");
  Serial.println(rms, 3);

  // 6. Send RMS to ThingSpeak if Wi-Fi is connected
  if (WiFi.status() == WL_CONNECTED) {
    ThingSpeak.setField(1, (float)rms);
    int result = ThingSpeak.writeFields(channelNumber, writeAPIKey);

    if (result == 200) {
      Serial.println("RMS sent to ThingSpeak.");
    } else {
      Serial.print("ThingSpeak write failed, code: ");
      Serial.println(result);
    }
  } else {
    Serial.println("Wi-Fi not connected. Skipping ThingSpeak upload.");
  }

  delay(20000);  // Minimum delay between uploads
}
