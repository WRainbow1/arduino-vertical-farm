#include <LiquidCrystal_I2C.h>

class LcdClient {
    private:
        LiquidCrystal_I2C lcd;
    public:
        LcdClient() : lcd(0x27, 20, 4) {
        }

        void init() {
            int vin_display = 22;
            pinMode(vin_display, OUTPUT);
            digitalWrite(vin_display, HIGH);

            lcd.init();
            lcd.backlight();
        }

        void light(int target, int actual) {
            lcd.setCursor(0, 0);
            lcd.print("ppfd T:");
            lcd.print(target);
            lcd.print("ppfd A:");
            lcd.print(actual);
        }

        void soil(
            int soil1M,
            int soil2M,
            int soil3M,
            int soil1A,
            int soil2A,
            int soil3A
        ) {
            lcd.setCursor(0, 1);
            lcd.print("B1 M:");
            lcd.print(soil1M);
            lcd.print("B3 A:");
            lcd.print(soil3A);

            lcd.setCursor(0, 2);
            lcd.print("B2 M:");
            lcd.print(soil2M);
            lcd.print("B3 A:");
            lcd.print(soil3A);
    
            lcd.setCursor(0, 3);
            lcd.print("B3 M:");
            lcd.print(soil3M);
            lcd.print("B3 A:");
            lcd.print(soil3A);
        }

        void last_water_time(unsigned long time_since_last) {
            lcd.setCursor(0, 3);
            lcd.print("Last water (h):");
            lcd.print(time_since_last);
        }
};
