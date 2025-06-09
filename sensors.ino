void readMoistureCapacitors(
    int sens_1,
    int sens_2,
    int sens_3,
    int v_in_1,
    int v_in_2,
    int v_in_3,
    int* out) {

    int in_pins[3] = {v_in_1, v_in_2, v_in_3};
    int sens_pins[3] = {sens_1, sens_2, sens_3};

    for (int i = 0; i < 1; i++) { //TODO change

        int in_pin = in_pins[i];
        int sens_pin = sens_pins[i];

        digitalWrite(in_pin, HIGH);

        delay(100);

        out_cps[i] = analogRead(sens_pin);

        digitalWrite(in_pin, LOW);
    }
}