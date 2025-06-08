#include "tests\test_actuators.h"

int lights = 6;
int pump_3 = 4;
int pump_2 = 3;
int pump_1 = 2;

void setup() {
  Serial.begin(9600);
  pinMode(lights, OUTPUT);
  pinMode(pump_1, OUTPUT);
  pinMode(pump_2, OUTPUT);
  pinMode(pump_3, OUTPUT);
}

void loop() {
  testOutputs(lights, pump_1, pump_2, pump_3);
}
