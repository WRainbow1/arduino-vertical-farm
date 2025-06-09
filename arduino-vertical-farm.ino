#include "tests\test_actuators.h"

//sensors
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
int pump_3 = 4;
int pump_2 = 3;
int pump_1 = 2;

void setup() {
  Serial.begin(9600);

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
  
  testOutputs(lights, pump_1, pump_2, pump_3);
  
  readMoistureCapacitors(
    sens_1,
    sens_2,
    sens_3,
    v_in_1,
    v_in_2,
    v_in_3,
    out_cps
  );

  Serial.println(out_cps[0]);

}
