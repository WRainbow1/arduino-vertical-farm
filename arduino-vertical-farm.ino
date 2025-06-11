#include "tests/test_actuators.h"
#include "clients/soil.h"
#include "clients/lcd.h"

// soil

int sens_1 = A0;
int sens_2 = A1;
int sens_3 = A2;

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

int soil_dry = 550; // quite dry


// light

int light_target = 0; // TODO change
int lights = 6;

LcdClient lcd;

PumpClient pc1(
  pump_1, 
  sens_1, 
  v_in_pin_1,
  interval,
  pump_time,
  pump_power,
  soil_dry
);

PumpClient pc2(
  pump_2, 
  sens_2, 
  v_in_pin_2,
  interval,
  pump_time,
  pump_power,
  soil_dry
);

PumpClient pc3(
  pump_3, 
  sens_3, 
  v_in_pin_3,
  interval,
  pump_time,
  pump_power,
  soil_dry
);


PumpClient pcArray[3] = {pc1, pc2, pc3};

void setup() {

  // actuators
  pinMode(lights, OUTPUT);
}

void loop() {

  for (int i = 0; i < 3; i++) {
    moisture_result = pcArray[i].moistureFeedbackLoop();
    lcd.bay(i+1);
    lcd.soil(moisture_result.cp);
    lcd.last_water_time(moisture_result.time_since_last);
  }

  analogWrite(lights, 100);

}

// testOutputs(lights, pump_1, pump_2, pump_3);
