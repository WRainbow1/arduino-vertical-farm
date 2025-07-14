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

        int wet = 300 / 4.0; // input voltage: completely waterlogged
        int dry = 650 / 4.0; // input voltage: bome dry

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
            Serial.begin(9600);
        }

    float readMoistureCapacitor() {
        
        float read = analogRead(sens_pin) / 4.0; // 255 scaled
        float range = dry - wet;
        float unflipped = (read - wet)*255.0/range; // scale so that range is over dry/wet = 255/0
        float result = 255 - unflipped; // switch so scale is wet/dry = 255/0, more intuitive

        // clamp
        if (result > 255) {
            return 255.0;
        } else if (result < 0) {
            return 0.0;
        }

        return result;
    }

    bool activatePump(float capacitor_val, float slider_val)   {
        // 255 / 255 power: // 1.2 L/Min

        // Serial.print("time delta: ");
        // Serial.println(millis() - last_time);
        // Serial.print("interval: ");
        // Serial.println(interval_ms);
        
        if ((millis() - last_time) >= interval_ms && capacitor_val < slider_val && slider_val > 10) {

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

        return {time_since_last, pump_ran, out_voltage / 2.5, max_voltage / 2.5};
    }

    float readSlider() {
        float slider_value;
        float max_voltage;
        slider_value = analogRead(slider_pin) / 4.0; // 255 scaled

        if (slider_value < 25.0) {
            slider_value = 0.0;
        }

        max_voltage = slider_value;
        return max_voltage;
    }
};
