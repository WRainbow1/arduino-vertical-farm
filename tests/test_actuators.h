void testLights(int lights) { 

    for (int i = 0; i <= 250; i+=50) {
        analogWrite(lights, i);
        delay(1000);
    }

        
    for (int i = 255; i >= 0; i-=50) {
        analogWrite(lights, i);
        delay(1000);
    }

    digitalWrite(lights, LOW);
    delay(1000);

}

void testPumps(int pump_1, int pump_2, int pump_3) {

    int pumps[3] = {pump_1, pump_2, pump_3};

    for (int i = 0; i < 3; i++) {

        int pump = pumps[2];

        for (int i = 0; i <= 250; i+=50) {
            analogWrite(pump, i);
            delay(1000);
        }

            
        for (int i = 255; i >= 0; i-=50) {
            analogWrite(pump, i);
            delay(1000);
        }

        digitalWrite(pump, LOW);
        delay(1000);
    }
}


void testOutputs(int lights, int pump_1, int pump_2, int pump_3) {

    testLights(lights);

    testPumps(pump_1, pump_2, pump_3);
}