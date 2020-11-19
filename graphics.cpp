#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


namespace graphics {

    typedef Adafruit_SSD1306 OLED_Display;

    #define SCREEN_WIDTH 128 // OLED display width, in pixels
    #define SCREEN_HEIGHT 32 // OLED display height, in pixels
    // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
    #define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)


    static OLED_Display display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

    inline void draw_routine() {
        display.
    }
}

