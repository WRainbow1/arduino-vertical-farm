#include <LiquidCrystal_I2C.h>

class LcdClient {
    private:
        LiquidCrystal_I2C lcd;
    public:
        LcdClient() : lcd(0x27, 20, 4) {
        }

        void init() {
            int vin_display = 24;
            pinMode(vin_display, OUTPUT);
            digitalWrite(vin_display, HIGH);

            lcd.init();
            lcd.backlight();
        }

        void light(int target, int actual) {
            lcd.setCursor(0, 0);
            lcd.print("L  M:");
            lcd.print(actual);
            lcd.setCursor(10, 0);
            lcd.print("T:");
            lcd.print(target);
        }

        void soil(
            int soil1M,
            int soil2M,
            int soil3M,
            int soil1T,
            int soil2T,
            int soil3T
        ) {
            lcd.setCursor(0, 1);
            lcd.print("B1 M:");
            lcd.print(soil1M);
            lcd.setCursor(10, 1);
            lcd.print("T:");
            lcd.print(soil1T);

            lcd.setCursor(0, 2);
            lcd.print("B2 M:");
            lcd.print(soil2M);
            lcd.setCursor(10, 2);
            lcd.print("T:");
            lcd.print(soil2T);
    
            lcd.setCursor(0, 3);
            lcd.print("B3 M:");
            lcd.print(soil3M);
            lcd.setCursor(10, 3);
            lcd.print("T:");
            lcd.print(soil3T);
        }

        void last_water_time(unsigned long time_since_last) {
            lcd.setCursor(0, 3);
            lcd.print("Last water (h):");
            lcd.print(time_since_last);
        }
};
