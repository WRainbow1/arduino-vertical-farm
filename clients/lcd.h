#include <LiquidCrystal_I2C.h>
#include <Wire.h>

class LcdClient {
    private:
        LiquidCrystal_I2C lcd;
    public:
        LcdClient() : lcd(0x27, 20, 4) {
            int vin_display = 22;
            pinMode(vin_display, OUTPUT);
            digitalWrite(vin_display, HIGH);

            Wire.begin();

            lcd.init();
            lcd.backlight();
        }

        void bay(int bay_num) {
            lcd.setCursor(0, 0);
            lcd.print("Bay:");
            lcd.print(bay_num);
        }

        void light(int lumens) {
            lcd.setCursor(0, 1);
            lcd.print("Lumens:");
            lcd.print(lumens);
        }

        void soil(int soilM) {
            lcd.setCursor(0, 2);
            lcd.print("SoilM:");
            lcd.print(soilM);
        }

        void last_water_time(unsigned long time_since_last) {
            lcd.setCursor(0, 3);
            lcd.print("Last water (h):");
            lcd.print(time_since_last);
        }
};
