#include <Adafruit_AS7341.h>
#include <PID_v1.h>

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
    PID* pid;  // pointer now

    const int light_pin = 2;
    const int sens_v_pin = 22;
    const int slider_v_pin = 23;
    const int slider_pin = A3;

    double sensor_constant = 1.0;

    double target_ppfd = 0;
    double actual_ppfd = 0;
    double output_pwm = 0;

    // PID tuning parameters
    double Kp = 0.3, Ki = 0.1, Kd = 0.001;

public:
    LightClient() : pid(nullptr) {}

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

        // Construct PID here AFTER Kp, Ki, Kd have been set
        pid = new PID(&actual_ppfd, &output_pwm, &target_ppfd, Kp, Ki, Kd, DIRECT);
        pid->SetOutputLimits(0, 255);
        pid->SetMode(AUTOMATIC);

        target_ppfd = 0;
        actual_ppfd = 0;
        output_pwm = 0;
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

    double getPPFD() {
        SensResult reading = readSensors();

        double cumulative_reading = (reading.F2 + reading.F8) * 200; // approximate
        double PPFD = cumulative_reading * sensor_constant;
        return PPFD;
    }

    double getSlider() {

        int raw_value = analogRead(slider_pin) / 4.0;
        return raw_value;
    }

    LightResult pidLoop() {
        target_ppfd = getSlider(); // scale 0-1023 to ~0-255 PWM range
        actual_ppfd = getPPFD();

        bool pid_result = pid->Compute();

        if (!pid_result) {
            Serial.println("PID compute returned false");
        }

        analogWrite(light_pin, (int)output_pwm);


        return {(int)target_ppfd, (int)actual_ppfd};
    }
};
