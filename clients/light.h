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

        int light_pin = 2;
        int sens_v_pin = 22;
        int slider_v_pin = 26;
        int slider_pin = A3;
        
        double sensor_constant = 1;
        double slider_constant = 1;

        double target_ppfd;
        double actual_ppfd;
        double output_pwm;

        double Kp = 2.0, Ki = 5.0, Kd = 1.0;
    
    public:
        LightClient() : pid(&actual_ppfd, &output_pwm, &target_ppfd, Kp, Ki, Kd, DIRECT) {}

    void init() {

        target_ppfd = 0;
        actual_ppfd = 0;
        output_pwm = 0;

        pinMode(sens_v_pin, OUTPUT);
        digitalWrite(sens_v_pin, HIGH);
        Serial.begin(9600);
        delay(100);
        Serial.println("starting");
        if (!sensor.begin()) {
            Serial.println("AS7341 not found!");
            while (1) delay(10);
        }
        Wire.setClock(100000);
        sensor.setATIME(100);
        sensor.setASTEP(999);
        sensor.setGain(AS7341_GAIN_256X);

        pid.SetOutputLimits(0, 255);
        pid.SetMode(AUTOMATIC);

        pinMode(slider_pin, INPUT);
        pinMode(light_pin, OUTPUT);

        pinMode(slider_v_pin, OUTPUT);
        digitalWrite(slider_v_pin, HIGH);
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
        digitalWrite(slider_v_pin, HIGH);
        target_ppfd = analogRead(slider_pin);
        digitalWrite(slider_v_pin, LOW);
        return target_ppfd;
    }
    
    LightResult pidLoop() {
        target_ppfd = getSlider() / 4.0;
        actual_ppfd = getPPFD();

        pid.Compute();

        Serial.print("output pwm ");
        Serial.println(output_pwm);
        // Serial.print("Chan 8 ");
        // Serial.println(result.F8);
    
        delay(10);  // Prevent hammering I²C
        analogWrite(light_pin, (int)output_pwm);

        return {(int)target_ppfd, (int)actual_ppfd};
    }
};