#include "tests/test_actuators.h"
#include "clients/soil.h"
#include "clients/lcd.h"
#include "clients/light.h"
#include <Wire.h>

// soil

int sens_1 = A0;
int sens_2 = A1;
int sens_3 = A2;
int slider_1 = A3;
int slider_2 = A4;
int slider_3 = A5;

int v_in_pin_1 = 30;
int v_in_pin_2 = 31;
int v_in_pin_3 = 32;

int pump_3 = 4; // 1.2 L/Min
int pump_2 = 3; 
int pump_1 = 2;

unsigned long interval = 30*60*1000; // 30 mins
int pump_time = 5000; // 5 seconds
int pump_power = 255;

MoistureResult moisture_result;
LightResult light_result;

int soil_dry = 550; // quite dry

// light

int lights = 6;

LcdClient lcd;

LightClient light;

PumpClient pc1(
  pump_1, 
  sens_1, 
  v_in_pin_1,
  interval,
  pump_time,
  pump_power,
  slider_1
);

PumpClient pc2(
  pump_2, 
  sens_2, 
  v_in_pin_2,
  interval,
  pump_time,
  pump_power,
  slider_2
);

PumpClient pc3(
  pump_3, 
  sens_3, 
  v_in_pin_3,
  interval,
  pump_time,
  pump_power,
  slider_3
);


PumpClient pcArray[3] = {pc1, pc2, pc3};

void setup() {
  Wire.begin();

  light.init();
  lcd.init();

}

void loop() {

  MoistureResult moisture_results[3]; 

  for (int i = 0; i < 3; i++) {
    moisture_results[i] = pcArray[i].moistureFeedbackLoop();
    //lcd.last_water_time(moisture_result.time_since_last);
  }

  light_result = light.pidLoop();

  lcd.soil(
    moisture_results[0].out_voltage,
    moisture_results[1].out_voltage,
    moisture_results[2].out_voltage,
    moisture_results[0].max_voltage,
    moisture_results[1].max_voltage,
    moisture_results[2].max_voltage
  );

  lcd.light(light_result.target_ppfd, light_result.actual_ppfd);
}

// testOutputs(lights, pump_1, pump_2, pump_3);
