#include <Adafruit_AS7341.h>

struct SensResult {
    float F2;
    float F8;
};

struct LightResult {
    int target_ppfd;
    int actual_ppfd;
};

class LightClient {
private:
    Adafruit_AS7341 sensor;

    const int light_pin = 2;
    const int sens_v_pin = 22;
    const int slider_v_pin = 23;
    const int slider_pin = A3;

    // measured from sens reading being 1.5 at ~ 500 umol/m^2/s
    float sensor_constant = 337.0;
    int max_ppfd = 500;

    double target_ppfd = 0;
    double actual_ppfd = 0;
    double output_pwm = 0;

    float pwm = 0;
    float gain = 0.00001;
    float integral_gain = 0.1;
    float error_sum = 0;

public:
    LightClient() {}

    void init() {
        Wire.begin();
        Serial.begin(9600);

        pinMode(sens_v_pin, OUTPUT);
        pinMode(slider_pin, INPUT);
        pinMode(slider_v_pin, OUTPUT);
        pinMode(light_pin, OUTPUT);

        digitalWrite(sens_v_pin, HIGH);
        digitalWrite(slider_v_pin, HIGH);

        delay(100);
        Serial.println("Starting sensor");

        if (!sensor.begin()) {
            Serial.println("AS7341 not found!");
            while (1) delay(10);
        }

        sensor.setATIME(1);
        sensor.setASTEP(999);
        sensor.setGain(AS7341_GAIN_256X);

        target_ppfd = 0;
        actual_ppfd = 0;
    }

    SensResult readSensors() {
        SensResult result;

        uint16_t readings[12];
        float counts[12];

        if (!sensor.readAllChannels(readings)){
            Serial.println("Error reading all channels!");
            return;
        }

        for(uint8_t i = 0; i < 12; i++) {
            if(i == 4 || i == 5) continue;
            // we skip the first set of duplicate clear/NIR readings
            // (indices 4 and 5)
            counts[i] = sensor.toBasicCounts(readings[i]);
        }

        result.F2 = counts[1];
        result.F8 = counts[9];

        return result;
    }

    float getPPFD() {
        SensResult reading = readSensors();

        float cumulative_reading = (reading.F2 + reading.F8);
        Serial.print("PPFD Raw Reading: ");
        Serial.println(cumulative_reading);
        float PPFD = cumulative_reading * sensor_constant;
        return PPFD;
    }

    float getSlider() {
        float raw_value = analogRead(slider_pin) / 4.0;
        if (raw_value < 10) {
            return 0;
        }
        float target_ppfd = raw_value*max_ppfd / 255.0;

        return target_ppfd;
    }

    LightResult pidLoop() {
        target_ppfd = getSlider();
        actual_ppfd = getPPFD();
        pwm = control_step(target_ppfd, actual_ppfd, pwm);

        analogWrite(light_pin, pwm);

        return {(int)target_ppfd, (int)actual_ppfd};
    }

    float control_step(float target_ppfd, float actual_ppfd, float current_pwm) {

        float ppfd_delta = target_ppfd - actual_ppfd;
        float error_sum = error_sum + ppfd_delta;

        float target_pwm = target_ppfd*255.0 / max_ppfd;
        float next_pwm = (current_pwm + abs(ppfd_delta)*ppfd_delta*gain + integral_gain*error_sum); 

        Serial.print("next pwm: ");
        Serial.println(next_pwm);

        if (next_pwm > 255) { // clamping
            next_pwm = 255;
            error_sum = 0;
        } else if (next_pwm < 10) {
            next_pwm = 0;
            error_sum = 0;
        }

        return next_pwm;
    }
};
