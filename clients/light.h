#include <Adafruit_AS7341.h>
#include <PID_v1.h>

struct SensResult {
    int F2;
    int F8;
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
    const int slider_v_pin = 26;
    const int slider_pin = A3;

    double sensor_constant = 1.0;

    double target_ppfd = 0;
    double actual_ppfd = 0;
    double output_pwm = 0;

    // PID tuning parameters
    double Kp = 2.0, Ki = 5.0, Kd = 1.0;

public:
    LightClient() : pid(nullptr) {}

    void init() {
        Serial.begin(9600);
        Wire.begin();
        pinMode(sens_v_pin, OUTPUT);
        digitalWrite(sens_v_pin, HIGH);

        pinMode(slider_pin, INPUT);
        pinMode(slider_v_pin, OUTPUT);
        digitalWrite(slider_v_pin, HIGH);

        pinMode(light_pin, OUTPUT);

        delay(100);
        Serial.println("Starting sensor");

        if (!sensor.begin()) {
            Serial.println("AS7341 not found!");
            while (1) delay(10);
        }

        Wire.setClock(100000);
        sensor.setATIME(100);
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
        result.F2 = sensor.getChannel(AS7341_CHANNEL_445nm_F2);
        delay(100);
        result.F8 = sensor.getChannel(AS7341_CHANNEL_680nm_F8);
        delay(100);
        return result;
    }

    double getPPFD() {
        SensResult reading = readSensors();

        double cumulative_reading = (reading.F2 + reading.F8) * 0.8; // approximate
        double PPFD = cumulative_reading * sensor_constant;
        return PPFD;
    }

    double getSlider() {
        digitalWrite(slider_v_pin, HIGH);
        int raw_value = analogRead(slider_pin);
        digitalWrite(slider_v_pin, LOW);
        return raw_value;
    }

    LightResult pidLoop() {
        target_ppfd = getSlider() / 4.0; // scale 0-1023 to ~0-255 PWM range
        actual_ppfd = getPPFD();

        Serial.print("Target PPFD: ");
        Serial.print(target_ppfd);
        Serial.print(" | Actual PPFD: ");
        Serial.println(actual_ppfd);

        bool pid_result = pid->Compute();

        if (!pid_result) {
            Serial.println("PID compute returned false");
        }

        Serial.print("Output PWM: ");
        Serial.println(output_pwm);

        analogWrite(light_pin, (int)output_pwm);

        delay(10);  // short delay to avoid I2C hammering

        return {(int)target_ppfd, (int)actual_ppfd};
    }
};
