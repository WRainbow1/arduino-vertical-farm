#include "tests/test_actuators.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// display
int vin_display = 22;
LiquidCrystal_I2C lcd(0x27, 20, 4);

// sensors
int sens_1 = A0;
int sens_2 = A1;
int sens_3 = A2;
int v_in_1 = 30;
int v_in_2 = 31;
int v_in_3 = 32;
int out_cps[3];

// sens boundaries
int soil_dry = 602; // quite dry
int soil_wet = 400; // very wet

// actuators
int lights = 6;
int pump_3 = 4; // 1.2 L/Min
int pump_2 = 3; // 1.2 L/Min
int pump_1 = 2; // 1.2 L/Min

void setup() {
  pinMode(vin_display, OUTPUT);
  digitalWrite(vin_display, HIGH);
  Serial.begin(9600);
  Wire.begin();
  
  // display
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("hello world");

  // sensors
  pinMode(sens_1, INPUT);
  pinMode(sens_2, INPUT);
  pinMode(sens_3, INPUT);
  pinMode(v_in_1, OUTPUT);
  pinMode(v_in_2, OUTPUT);
  pinMode(v_in_3, OUTPUT);

  // actuators
  pinMode(lights, OUTPUT);
  pinMode(pump_1, OUTPUT);
  pinMode(pump_2, OUTPUT);
  pinMode(pump_3, OUTPUT);
}

void loop() {
  lcd.setCursor(0, 1);
  lcd.print("Counter:");
  lcd.print(millis() / 1000);

  // testOutputs(lights, pump_1, pump_2, pump_3);
  
  // readMoistureCapacitors(
  //   sens_1,
  //   sens_2,
  //   sens_3,
  //   v_in_1,
  //   v_in_2,
  //   v_in_3,
  //   out_cps
  // );

}
