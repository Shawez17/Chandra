#include "Arduino.h"
#include "X9C10X.h"
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ================= OLED =================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

// RST remains unchanged / not controlled by ESP32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ================= X9C10X =================
X9C10X pot(12345);  // 100KΩ

// ================= ADS1115 =================
Adafruit_ADS1115 ads;

// ================= PINS =================
const uint8_t INC_PIN = 25;
const uint8_t UD_PIN  = 27;
const uint8_t CS_PIN  = 33;

const uint8_t ADS_ADDR = 0x48;

// ================= VOLTAGE =================
const float VL = 0.0;
const float VH = 5.0;

const uint8_t MAX_POS = 99;

// ADS1115 GAIN_TWOTHIRDS
const float ADS_LSB_VOLTS = 0.0001875;

const int RAMP_DELAY = 100;

// ======================================================

void setup()
{
  Serial.begin(115200);
  while (!Serial);

  Serial.println();
  Serial.println(__FILE__);
  Serial.print("X9C10X_LIB_VERSION: ");
  Serial.println(X9C10X_LIB_VERSION);
  Serial.println();

  // ---------------- X9C10X ----------------
  Serial.println(pot.getType());

  pot.begin(INC_PIN, UD_PIN, CS_PIN);
  pot.setPosition(0);

  // ---------------- I2C ----------------
  Wire.begin();

  // ---------------- OLED ----------------
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR))
  {
    Serial.println("OLED not found!");
    while (1);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println("X9C10X + ADS1115");
  display.println();
  display.println("Initializing...");
  display.display();

  // ---------------- ADS1115 ----------------
  if (!ads.begin(ADS_ADDR))
  {
    Serial.println("ADS1115 not found - check wiring/address");

    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("ADS1115 ERROR!");
    display.println();
    display.println("Check wiring");
    display.display();

    while (1)
    {
      delay(10);
    }
  }

  ads.setGain(GAIN_TWOTHIRDS);

  Serial.println();
  Serial.println("Ramping VW from 0V to 5V continuously...");
  Serial.println("Position\tSet Voltage\tADS1115 raw\tActual VW");
  Serial.println();

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Ramping VW");
  display.println("0V -> 5V");
  display.display();

  delay(1000);
}

// ======================================================

void loop()
{
  // Ramp UP: 0 -> 99
  for (uint8_t position = 0; position <= MAX_POS; position++)
  {
    rampStep(position);
  }

  delay(500);

  // Ramp DOWN: 99 -> 0
  for (int position = MAX_POS; position >= 0; position--)
  {
    rampStep(position);
  }

  delay(500);
}

// ======================================================

void rampStep(uint8_t position)
{
  // Set potentiometer position
  pot.setPosition(position);

  // Allow wiper and ADC to settle
  delay(30);

  // Calculate expected voltage
  float setVoltage =
      VL + (VH - VL) * ((float)position / MAX_POS);

  // Read ADS1115 A3
  int16_t raw = ads.readADC_SingleEnded(3);

  // Convert ADC reading to voltage
  float actualVoltage = raw * ADS_LSB_VOLTS;

  // ==================================================
  // SERIAL OUTPUT
  // ==================================================

  Serial.print("Position: ");
  Serial.print(position);

  Serial.print("\tSet: ");
  Serial.print(setVoltage, 3);
  Serial.print(" V");

  Serial.print("\tRaw: ");
  Serial.print(raw);

  Serial.print("\tVW: ");
  Serial.print(actualVoltage, 3);
  Serial.println(" V");

  // ==================================================
  // OLED OUTPUT
  // ==================================================

  display.clearDisplay();

  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println("X9C10X RAMP");

  display.setCursor(0, 12);
  display.print("Position: ");
  display.println(position);

  display.setCursor(0, 24);
  display.print("Set: ");
  display.print(setVoltage, 3);
  display.println(" V");

  display.setCursor(0, 36);
  display.print("ADC: ");
  display.println(raw);

  display.setCursor(0, 48);
  display.print("VW: ");
  display.print(actualVoltage, 3);
  display.println(" V");

  display.display();

  // Wait before next position
  delay(RAMP_DELAY);
}
