struct MoistureResult {
    unsigned long time_since_last; //hours
    int cp;
};

class PumpClient {
    private:
        int pump_pin;
        int sens_pin;
        int sens_vin_pin;

        unsigned long interval_ms;
        unsigned long last_time;

        int pump_time_ms;
        int power;
        int cp_dry_threshold;

    public:
        PumpClient(
            int pump, int sens, int sens_vin_pin,
            unsigned long interval,
            int pump_time,
            int pump_power,
            int cp_dry_threshold
        ) {
            pump_pin = pump;
            sens_pin = sens;
            sens_vin_pin = sens_vin_pin;

            interval_ms = interval;
            last_time = interval;

            pump_time_ms = pump_time;
            power = pump_power;
            cp_dry_threshold = cp_dry_threshold;

            pinMode(pump_pin, OUTPUT);
            pinMode(sens_pin, INPUT);
            pinMode(sens_vin_pin, OUTPUT);
        }

    MoistureResult moistureFeedbackLoop() {
        // 255 / 255 power: // 1.2 L/Min

        int out_cp = readMoistureCapacitor();

        activatePump(out_cp);

        unsigned long time_since_last = (millis() - last_time)/(1000UL*3600UL);

        return {time_since_last, out_cp};
    }


    int readMoistureCapacitor() {

        digitalWrite(sens_vin_pin, HIGH);
        delay(100);

        int out_cp = analogRead(sens_pin);
        digitalWrite(sens_vin_pin, LOW);

        return out_cp;
    }

    unsigned long activatePump(int out_cp)   {
        if ((millis() - last_time) >= interval_ms || out_cp > cp_dry_threshold) {
            analogWrite(pump_pin, power);
            delay(pump_time_ms);
            digitalWrite(pump_pin, LOW);

            last_time = millis();
        }
    }

};
