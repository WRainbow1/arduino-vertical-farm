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
        PID pid;

        int light_pin = 10;
        int slider_pin = A7;
        
        double sensor_constant = 1;
        double slider_constant = 1;

        double target_ppfd;
        double actual_ppfd;
        double output_pwm;

        double Kp = 2.0, Ki = 5.0, Kd = 1.0;
    
    public:
        LightClient() : pid(&actual_ppfd, &output_pwm, &target_ppfd, Kp, Ki, Kd, DIRECT) {}

    void init() {
        sensor.begin();
        sensor.setATIME(100);
        sensor.setASTEP(999);
        sensor.setGain(AS7341_GAIN_256X);

        pid.SetOutputLimits(0, 255);

        pinMode(slider_pin, INPUT);
        pinMode(light_pin, OUTPUT);
    }
    
    SensResult readSensors() {
        SensResult result;
        result.F2 = sensor.getChannel(AS7341_CHANNEL_445nm_F2);
        result.F8 = sensor.getChannel(AS7341_CHANNEL_680nm_F8);
        return result;
    }

    double getPPFD() {
        SensResult reading;
        double cumulative_reading;
        double PPFD;
        reading = readSensors();
        
        cumulative_reading = (reading.F2 + reading.F8) * 0.8; //not bang on sens wavelengths
        PPFD = cumulative_reading * sensor_constant;
        return PPFD;
    }

    double getSlider() {
        int target_ppfd;
        target_ppfd = analogRead(slider_pin);
        return target_ppfd;
    }
    
    LightResult pidLoop() {
        target_ppfd = getSlider();
        actual_ppfd = getPPFD();

        pid.Compute();
        analogWrite(light_pin, (int)output_pwm);

        return {(int)target_ppfd, (int)actual_ppfd};
    }
};