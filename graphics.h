
#ifndef GRAPHICS_H
#define GRAPHICS_H



namespace graphics_implementation {

    typedef Adafruit_SSD1306 OLED_Display;

    #define SCREEN_WIDTH 128 // OLED display width, in pixels
    #define SCREEN_HEIGHT 32 // OLED display height, in pixels
    // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
    #define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)


    static OLED_Display mini_display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
    static U8G2_ST7920_128X64_1_SW_SPI u8g2(/*rotation = U8G2_R0*/ U8G2_R2, /* clock=*/ 13, /* data=*/ 11, /* CS=*/ 12, /* reset=*/ 8);

    void pre_update()
    {
        u8g2.setFont(u8g2_font_6x10_tf);
        u8g2.setFontRefHeightExtendedText();
        u8g2.setDrawColor(1);
        u8g2.setFontPosTop();
        u8g2.setFontDirection(2); // 180 deg rotation
    }

    void initialize_screens()
    {
        u8g2.begin();
    }

    inline void draw_box(u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h)
    {
        u8g2.drawBox(x, y, w, h);
    }
    inline void draw_box(float _x, float _y, u8g2_uint_t w, u8g2_uint_t h)
    {
        const u8g2_uint_t x = round(_x);
        const u8g2_uint_t y = round(_y);

        u8g2.drawBox(x, y, w, h);
    }
    inline void draw_frame(u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h)
    {
        u8g2.drawFrame(x, y, w, h);
    }
    inline void draw_frame(float _x, float _y, u8g2_uint_t w, u8g2_uint_t h)
    {
        const u8g2_uint_t x = round(_x);
        const u8g2_uint_t y = round(_y);
        u8g2.drawFrame(x, y, w, h);
    }

    inline void clear()
    {
        u8g2.clear();
    }

    inline void initial_refresh()
    {
        u8g2.firstPage();
    }
    inline uint8_t non_terminating_refresh()
    {
        return u8g2.nextPage();
    }
    
    inline void draw_string(u8g2_uint_t x, u8g2_uint_t y, const char *s)
    {
        u8g2.drawStr(x, y, s);
    }

    #ifndef ARDUINO_H
    typedef unsigned char byte;
    #endif

    namespace sprites
    {
        static const byte person_run_frames[2][8] = {
        {
            B01100,
            B01100,
            B00000,
            B01110,
            B11100,
            B01100,
            B11010,
            B10011
        },
        {
            B01100,
            B01100,
            B00000,
            B01100,
            B01100,
            B01100,
            B01100,
            B01110
        }
        };
    }
}

#endif