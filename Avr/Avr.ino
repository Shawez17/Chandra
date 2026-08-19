#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MCP4725.h>

Adafruit_MCP4725 dac;
const uint8_t MCP4725_ADDR = 0x60;


#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64   // change to 32 if your module is smaller
#define OLED_ADDR 0x3C
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);
  while (!Serial) { ; }

  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("SSD1306 not found - check wiring/address"));
    while (1) { delay(10); }
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("OLED ready"));
  display.println(F("SSD1306 @ 0x3C"));
  display.display();

   if (!dac.begin(MCP4725_ADDR)) {
    Serial.println(F("MCP4725 not found - check wiring/address"));
    while (1) { delay(10); }
  }

  Serial.println(F("MCP4725 DAC initialized"));

}

void loop() {
  static unsigned long lastUpdate = 0;
  static uint16_t counter = 0;

  if (millis() - lastUpdate >= 1000) {
    lastUpdate = millis();
    counter++;

    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(F("Counter:"));
    display.setTextSize(2);
    display.setCursor(0, 16);
    display.println(counter);
    display.display();
  }
  for (uint16_t val = 0; val <= 4095; val += 64) {
    dac.setVoltage(val, false); // false = RAM only, not EEPROM
    delay(20);
  }
  for (uint16_t val = 4095; val > 0; val -= 64) {
    dac.setVoltage(val, false);
    delay(20);
  }





}
