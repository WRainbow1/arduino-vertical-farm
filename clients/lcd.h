#include <LiquidCrystal_I2C.h>

class LcdClient {
    private:
        LiquidCrystal_I2C lcd;

        void print_left_padded(int value, int width, bool with_percent = false) {
            String s = String(value);
            if (with_percent) {
                s += "%";
            }
            while (s.length() < width) {
                s += " ";
            }
            lcd.print(s);
        }

    public:
        LcdClient() : lcd(0x27, 20, 4) {}

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
            print_left_padded(actual, 4);  // no percent
            lcd.setCursor(10, 0);
            lcd.print("T:");
            print_left_padded(target, 4);  // no percent

            if (target < 10) {
                lcd.noBacklight();
            } else {
                lcd.backlight();
            }
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
            print_left_padded(soil1M, 5, true);  // with %
            lcd.setCursor(10, 1);
            lcd.print("T:");
            print_left_padded(soil1T, 5, true);

            lcd.setCursor(0, 2);
            lcd.print("B2 M:");
            print_left_padded(soil2M, 5, true);
            lcd.setCursor(10, 2);
            lcd.print("T:");
            print_left_padded(soil2T, 5, true);

            lcd.setCursor(0, 3);
            lcd.print("B3 M:");
            print_left_padded(soil3M, 5, true);
            lcd.setCursor(10, 3);
            lcd.print("T:");
            print_left_padded(soil3T, 5, true);
        }

        void last_water_time(unsigned long time_since_last) {
            lcd.setCursor(0, 3);
            lcd.print("Last water (h):");
            print_left_padded((int)time_since_last, 4);  // no percent
        }
};
