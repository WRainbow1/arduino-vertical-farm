struct MoistureResult {
    unsigned long time_since_last; //hours
    bool pump_ran;
    int out_voltage;
    int max_voltage;
};

class PumpClient {
    private:
        int pump_pin;
        int sens_pin;
        int sens_vin_pin;
        int slider_v_in;
        int slider_pin;

        unsigned long interval_ms;
        unsigned long last_time = 0;

        int pump_time_ms;
        int power;
        double slider_scaling_const = 1;

    public:
        PumpClient(
            int pump, int sens, int sens_vin_pin,
            unsigned long interval,
            int pump_time,
            int pump_power,
            int slider_v_in,
            int slider_pin
        ) {
            this->pump_pin = pump;
            this->sens_pin = sens;
            this->sens_vin_pin = sens_vin_pin;

            this->interval_ms = interval;
            this->last_time = interval;

            this->pump_time_ms = pump_time;
            this->power = pump_power;
            this->slider_pin = slider_pin;
            this->slider_v_in = slider_v_in;


            pinMode(this->pump_pin, OUTPUT);
            pinMode(this->sens_pin, INPUT);
            pinMode(this->sens_vin_pin, OUTPUT);
            pinMode(this->slider_v_in, OUTPUT);
            pinMode(this->slider_pin, INPUT);

            digitalWrite(this->sens_vin_pin, HIGH);
            digitalWrite(this->slider_v_in, HIGH);
        }

    int readMoistureCapacitor() {
        
        int out_voltage= analogRead(sens_pin);
        return out_voltage;
    }

    bool activatePump(int out_cp, int max_voltage)   {
        // 255 / 255 power: // 1.2 L/Min
        
        if (((millis() - last_time) >= interval_ms || out_cp > max_voltage) && false) {
            analogWrite(pump_pin, power);
            delay(pump_time_ms);
            digitalWrite(pump_pin, LOW);

            last_time = millis();
            return true;
        }
        return false;
    }

    MoistureResult moistureFeedbackLoop() {

        int out_voltage = readMoistureCapacitor();
        int max_voltage = readSlider();
        bool pump_ran = activatePump(out_voltage, max_voltage);
        unsigned long time_since_last = (millis() - last_time)/(1000UL*3600UL);

        return {time_since_last, pump_ran, out_voltage, max_voltage};
    }

    int readSlider() {
        int slider_value;
        int max_voltage;
        slider_value = analogRead(slider_pin) / 4.0;

        max_voltage = slider_value / slider_scaling_const;
        return max_voltage;
    }
};
