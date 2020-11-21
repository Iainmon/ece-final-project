
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

    // static const byte_t default_font_mode = u8g2_font_6x10_tf;

    void pre_update()
    {
        u8g2.setFont(u8g2_font_6x10_tf);
        u8g2.setFontRefHeightExtendedText();
        u8g2.setDrawColor(1);
        u8g2.setBitmapMode(1);
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
    inline void draw_box(float &_x, float &_y, u8g2_uint_t w, u8g2_uint_t h)
    {
        const u8g2_uint_t x = round(_x);
        const u8g2_uint_t y = round(_y);

        u8g2.drawBox(x, y, w, h);
    }
    inline void draw_frame(u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t w, u8g2_uint_t h)
    {
        u8g2.drawFrame(x, y, w, h);
    }
    inline void draw_frame(float &_x, float &_y, u8g2_uint_t w, u8g2_uint_t h)
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

    inline void draw_glyph(u8g2_uint_t x, u8g2_uint_t y, uint16_t encoding)
    {
        u8g2.drawGlyph(x, y, encoding);
    }
    inline void draw_glyph(float &_x, float &_y, uint16_t encoding)
    {
        const u8g2_uint_t x = round(_x);
        const u8g2_uint_t y = round(_y);
        // u8g2.setFont(u8g2_font_unifont_t_symbols);
        // u8g2.drawGlyph(x, y, encoding);
        // u8g2.setFont(u8g2_font_6x10_tf);
    }

    inline void draw_bitmap(float &_x, float &_y, u8g2_uint_t cnt, u8g2_uint_t h, const uint8_t *bitmap)
    {
        const u8g2_uint_t x = round(_x);
        const u8g2_uint_t y = round(_y);
        u8g2.drawBitmap(x, y, cnt, h, bitmap);
    }
    inline void draw_bitmap(u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t cnt, u8g2_uint_t h, const uint8_t *bitmap)
    {
        u8g2.drawBitmap(x, y, cnt, h, bitmap);
    }


    namespace sprites
    {
        static const byte_t person_run_frames[2][12] = {
        {
            B00000000,
            B00111000,
            B00111000,
            B00111000,
            B00010010,
            B00111100,
            B01111000,
            B10111000,
            B00110000,
            B01101000,
            B01000100,
            B10001000,

            // B10001000,
            // B01000100,
            // B01101000,
            // B00110000,
            // B10111000,
            // B01111000,
            // B00111100,
            // B00010010,
            // B00111000,
            // B00111000,
            // B00111000,
            // B00000000,

        },
        {
            B00000000,
            B00111000,
            B00111000,
            B00111000,
            B00010000,
            B01111000,
            B00111100,
            B00111000,
            B00110000,
            B00101000,
            B00101000,
            B01010000,

            // B01010000,
            // B00101000,
            // B00101000,
            // B00110000,
            // B00111000,
            // B01111000,
            // B00111100,
            // B00010000,
            // B00111000,
            // B00111000,
            // B00111000,
            // B00000000,
        }
        };

        // 
        // This is monkey code.
        // 
        inline byte_t reverse_byte(byte_t b) 
        {
            b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
            b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
            b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
            return b;
        }
        inline void reverse_array(byte_t arr[], byte_t start, byte_t end)
        {
            while (start < end)
            {
                byte_t temp = arr[start];
                arr[start] = arr[end];
                arr[end] = temp;
                start++;
                end--;
            }
        }
        inline void reverse_all_bytes(byte_t array[], byte_t n)
        {
            for (byte_t i = 0; i < n; ++i) {
                array[i] = reverse_byte(array[i]);
            }
        }
        inline void format_sprites()
        {
            reverse_array(person_run_frames[0], 0, 12);
            reverse_all_bytes(person_run_frames[0], 12);
            reverse_array(person_run_frames[1], 0, 12);
            reverse_all_bytes(person_run_frames[1], 12);
        }
    }
}

#endif