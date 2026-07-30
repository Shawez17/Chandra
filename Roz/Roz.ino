#include<Adafruit_MCP4725.h>
#include<Wire.h>
#include<Adafruit_SSD1306.h>
#include "Wire.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


Adafruit_MCP4725 dac;
#define DAC_RESOLUTION (12)
const int scl = 22;
const int sda = 23;
const int led_a = 21;
const int led_b = 16;
const int CLK_a = 26;
const int SW_a = 33;
const int CLK_b = 18;
//const int SW_b = ??
const int DT_a = 35;
const int DT_b = 19;
const int V_a = 34;
//const int V_b = SP;
const int I_a = 32;
//const int I_b = SM;



#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1   // Reset pin (-1 if sharing Arduino reset pin, common when no dedicated reset pin exists)
#define SCREEN_ADDRESS 0x3C // Common address; some boards use 0x3D — check your I2C scanner output

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



void setup(){
  Serial.begin(115600);
  Wire.begin(sda,scl);
  Serial.println("Done");
  pinMode(led_a, OUTPUT);
  pinMode(led_b, OUTPUT);
  
  digitalWrite(led_a, HIGH);
  digitalWrite(led_b, HIGH);


}
void loop(){
  Serial.println("Jim and Maggie");
  delay(500);
  Serial.println(DAC_RESOLUTION);
  //int print = dac.setVoltage(value,true);
  //Serial.println(print);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
      Serial.println("SSD1306 allocation failed");
      while (true); // halt if display isn't found
    }
  byte error, address;
  int nDevices = 0;

  delay(5000);

  Serial.println("Scanning for I2C devices ...");
  for (address = 0x01; address < 0x7f; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.printf("I2C device found at address 0x%02X\n", address);
      nDevices++;
    } else if (error != 2) {
      Serial.printf("Error %u at address 0x%02X\n", error, address);
    }
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found, Now liev your life with this!");
  }


  display.clearDisplay();
  display.setTextSize(2);          // adjust size as needed
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);        // x, y position
  display.println("HULK");
  display.display();               // must call this to actually push the buffer to screen
}



