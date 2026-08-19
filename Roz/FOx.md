#include<Adafruit_MCP4725.h> // DAC X
#include<Wire.h> //i2c
#include<Adafruit_SSD1306.h> // oled display
#include <Adafruit_GFX.h> // oled display
#include <Arduino.h> // for arduino internal function
#include <WiFi.h>
#include <WebServer.h>

// Website
const char* AP_SSID = "HULK-HV";
const char* AP_PASSWORD = "12345678";
// WebServer server(80);



Adafruit_MCP4725 dac;
#define DAC_RESOLUTION (12)
#define ADC_RESOLUTION (12) // assumed 
// -- define pins --
// i2c pins for DACs & oled display
const int scl = 22;
const int sda = 23;
// adc pins for HV module
const int v_mon_1 = 36;
const int i_mon_1 = 39;
const int v_mon_2 = 34;
const int i_mon_2 = 32;
// Gpio pins for rotatary encoder(problem with J3)
const int dt_1 = 35;
const int clk_1 = 26;
const int sw_1 = 33;
const int dt_2 = 19;
const int clk_2 = 18;
const int sw_2 = 17;//problem
// -- define pins --

//Monitoring logic


const float ADC_MAX_VOLTAGE = 3.3;
const float MODULE_MAX_VOLTAGE = 5.0;
const int ADC_MAX = 4095;
const float HV_MAX = 6000.0;
const float CURRENT_MAX_uA = 670.0; 

float adcToVoltage(int adcValue){
  int adcVoltage = adcValue/(float)ADC_MAX * ADC_MAX_VOLTAGE;
  return adcVoltage;
}
float adcToModuleVoltage(float adcVoltage){
  int moduleVoltage = adcVoltage/(float)ADC_MAX_VOLTAGE * MODULE_MAX_VOLTAGE;
  return moduleVoltage;
}

float moduleVoltageToHV(float moduleVoltage){
  moduleVoltage = constrain(
    moduleVoltage,
    0.0,
    MODULE_MAX_VOLTAGE
  );
  return (moduleVoltage / MODULE_MAX_VOLTAGE)* HV_MAX;
}

float moduleVoltageToCurrent(float moduleVoltage){
  moduleVoltage = constrain(
      moduleVoltage,
      0.0,
      MODULE_MAX_VOLTAGE
  );
  return (moduleVoltage / MODULE_MAX_VOLTAGE)* CURRENT_MAX_uA;
}



// Vset logic #1
const int DAC_MAX_CODE = 4095;
const float DAC_MAX_VOLTAGE = 5.0;
const float HV_MAX_Set = 6000.0;
const float VSET_MAX = 5.0;

float hvToVset(float desiredHV){

  desiredHV=constrain(desiredHV,0.0, HV_MAX_Set);
  float vSet = (desiredHV / HV_MAX_Set) * VSET_MAX;

  return vSet;
}

float vSetToDAC(float vSet){
  vSet = constrain(vSet,0.0, VSET_MAX);
  int dacCode = round((vSet / VSET_MAX) * DAC_MAX_CODE);

  return dacCode;
}



#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1   // Reset pin (-1 if sharing Arduino reset pin, common when no dedicated reset pin exists)
#define SCREEN_ADDRESS 0x3C // Common address; some boards use 0x3D — check your I2C scanner output

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



void setup(){
  Serial.begin(115600);
  Wire.begin(sda,scl);
  Serial.println("Controlling ,HV Control Interface");
  /*
  pinMode(led_a, OUTPUT);
  pinMode(led_b, OUTPUT);
  
  digitalWrite(led_a, HIGH);
  digitalWrite(led_b, HIGH);
  
  analogReadResolution(ADC_RESOLUTION);
  WiFi.mode(WIFI_AP);
      WiFi.softAP(
          AP_SSID,
          AP_PASSWORD

      );

  Serial.println(DAC_RESOLUTION);
  Serial.print(
        "SSID: "
    );

  Serial.println(
      AP_SSID
  );

  Serial.print(
      "IP address: "
  );

  Serial.println(
      WiFi.softAPIP()
  );

  server.begin();

  Serial.println(
        "Web server started."
    );

*/

  }
void loop(){
  //server.handleClient(); // ahndle incomign client
    //int print = dac.setVoltage(value,true);
  //Serial.println(print);
  //Init Module
  int adc_v1 = analogRead(v_mon_1);
  int adc_v2 = analogRead(v_mon_2);
  int adc_i1 = analogRead(i_mon_1);
  int adc_i2 = analogRead(i_mon_2);
  
  float adcVoltage_v1 = adcToVoltage(adc_v1);
  float adcVoltage_v2 = adcToVoltage(adc_v2);
  float adcVoltage_i1 = adcToVoltage(adc_i1);
  float adcVoltage_i2 = adcToVoltage(adc_i2);
  
  float moduleVoltage_v1 = adcToModuleVoltage(adcVoltage_v1);
  float moduleVoltage_v2 = adcToModuleVoltage(adcVoltage_v2);
  float moduleVoltage_i1 = adcToModuleVoltage(adcVoltage_i1);
  float moduleVoltage_i2 = adcToModuleVoltage(adcVoltage_i2);

  float kV1 = moduleVoltageToHV(moduleVoltage_v1);
  float kV2 = moduleVoltageToHV(moduleVoltage_v2);

  float uA1 = moduleVoltageToCurrent(moduleVoltage_i1);
  float uA2 = moduleVoltageToCurrent(moduleVoltage_i2);

  // printing monitoring data extracted from module
  Serial.printf("ADC analog value = %d\n", adc_v1);
  Serial.printf("ADC voltage  = %d\n", adcVoltage_v1);
  Serial.printf("Module Voltage = %d\n", moduleVoltage_v1);
  Serial.printf("kV Voltage = %d\n", kV1);
  Serial.printf("uA Current = %d\n", uA1);

  delay(500);
  
  byte error, address;
  int nDevices = 0;

// finding any i2c devices
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
      Serial.println("No I2C devices found!");
    }

  // finding MCP4725 
    if (!dac.begin(0x60)) {
      Serial.println("MCP4725 not found!");
      while (1);
  }

  // finding oled display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
      Serial.println("SSD1306 allocation failed");
      while (true); // halt if display isn't found
    }


  //Hv Control interface
  Serial.println("prototype of HV control");
  float desiredHV = 3000.0;
  float vSet = hvToVset(desiredHV);
  int dacCode = vSetToDAC(vSet);
  dac.setVoltage(dacCode, false);
  Serial.print("Desired HV: ");

  Serial.print(desiredHV);

  Serial.println(" V");

  Serial.print("Required V-SET: ");

  Serial.print(vSet, 4);

  Serial.println(" V");

  Serial.print("DAC code: ");

  Serial.println(dacCode);
  

  delay(5000);
  

  // display code
  display.clearDisplay();
  display.setTextSize(1);          // adjust size as needed
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);        // x, y position
  display.println("HULK");
  display.display();               // must call this to actually push the buffer to screen




}



