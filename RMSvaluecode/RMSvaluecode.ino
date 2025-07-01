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
const char* ssid = "EAJ S10+";
const char* password = "vivalavida";

// ThingSpeak credentials
const char* writeAPIKey = "VYT2XSSB4NB7WQJF";
unsigned long channelNumber = 2989950;

WiFiClient client;

void setup() {
  Serial.begin(115200);
  Wire.begin();
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
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected.");

  ThingSpeak.begin(client);
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

  // 4. Resetting Serial Plotter: print one spectrum per frame
  // Frequency per bin: (sampling frequency) / SAMPLES
  double frequencyStep = SAMPLING_FREQUENCY / SAMPLES;

  for (int i = 1; i < SAMPLES / 2; i++) {  // skip DC (bin 0)
    double freq = i * frequencyStep;
    Serial.print(freq, 1);
    Serial.print("\t");
    Serial.println(vReal[i], 3);
  }

  // 5. Send RMS value to ThingSpeak
  ThingSpeak.setField(1, (float)rms);
  int result = ThingSpeak.writeFields(channelNumber, writeAPIKey);

  if (result == 200) {
    Serial.print("RMS sent to ThingSpeak: ");
    Serial.println(rms, 3);
  } else {
    Serial.print("ThingSpeak write failed, code: ");
    Serial.println(result);
  }

  delay(20000);  // ThingSpeak minimum update interval
}