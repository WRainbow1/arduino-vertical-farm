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

    double sensor_constant = 1.0;

    double target_ppfd = 0;
    double actual_ppfd = 0;
    double output_pwm = 0;
    int pwm = 0;

    // PID tuning parameters

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

        // TODO: add mapping function from slider value to max/min ppfd

        return result;
    }

    double getPPFD() {
        SensResult reading = readSensors();

        double cumulative_reading = (reading.F2 + reading.F8) * 100; // approximate, 0.8 mult for 
        double PPFD = cumulative_reading * sensor_constant;
        return PPFD;
    }

    double getSlider() {
        int raw_value = analogRead(slider_pin) / 4.0;
        if (raw_value < 5) {
            return 0;
        }
        // TODO: add mapping function from slider value to max/min ppfd
        return raw_value;
    }

    LightResult pidLoop() {
        target_ppfd = getSlider();
        actual_ppfd = getPPFD();
        pwm = control_step(target_ppfd, actual_ppfd, pwm);

        analogWrite(light_pin, pwm);

        return {(int)target_ppfd, (int)actual_ppfd};
    }

    int control_step(int target_ppfd, int actual_ppfd, int current_pwm) {

        int ppfd_delta = target_ppfd - actual_ppfd;

        int target_pwm = target_ppfd; // add mapping back from ppfd to pwm
        int next_pwm = (current_pwm + ppfd_delta / 5); //iterate to it in 5 steps
        
        Serial.print("pwm ");
        Serial.println(next_pwm);


        if (next_pwm > 255) { // clamping
            next_pwm = 255;
        } else if (next_pwm < 0) {
            next_pwm = 0;
        }

        return next_pwm;
    }
};
