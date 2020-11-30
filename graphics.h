// ---------------------------------------------
// Created by Iain Moncrief on November 13, 2020
// Arduino Game - game.cpp
// ---------------------------------------------

#ifndef GRAPHICS_H
#define GRAPHICS_H



namespace graphics_implementation {

    typedef Adafruit_SSD1306 OLED_Display;
    typedef U8G2_ST7920_128X64_1_SW_SPI LCD_Display;

    #define SCREEN_WIDTH 128 // OLED display width, in pixels
    #define SCREEN_HEIGHT 64 // OLED display height, in pixels
    // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
    #define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
    #define OLED_ADDY 0x3C


    static OLED_Display mini_display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
    // static OLED_Display mini_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    static LCD_Display u8g2(/*rotation = U8G2_R0*/ U8G2_R2, /* clock=*/ 13, /* data=*/ 11, /* CS=*/ 12, /* reset=*/ 8);

    #define DEFAULT_FONT_MODE u8g2_font_tenstamps_mu // u8g2_font_6x10_tf;
    #define DEBUG_FONT_MODE u8g2_font_5x8_mf

    void pre_update()
    {
        u8g2.setFont(DEFAULT_FONT_MODE);
        u8g2.setFontRefHeightExtendedText();
        u8g2.setDrawColor(1);
        u8g2.setBitmapMode(1);
        u8g2.setFontPosTop();
        u8g2.setFontDirection(2); // 180 deg rotation

        // mini_display.clearDisplay();
        //mini_display.drawTriangle(63, 0, 0, 63, 127, 63, WHITE);
    }

    void post_update() {
        mini_display.display();
    }

    void initialize_screens()
    {
        u8g2.begin();
        mini_display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDY);
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
    inline void debug_draw_string(u8g2_uint_t x, u8g2_uint_t y, const char *s)
    {
        u8g2.setFont(DEBUG_FONT_MODE);
        u8g2.drawStr(x, y, s);
        u8g2.setFont(DEFAULT_FONT_MODE);
    }

    inline void draw_glyph(u8g2_uint_t x, u8g2_uint_t y, uint16_t encoding)
    {
        u8g2.drawGlyph(x, y, encoding);
    }
    inline void draw_glyph(float _x, float _y, uint16_t encoding)
    {
        u8g2.setFont(u8g2_font_cursor_tr);
        const u8g2_uint_t x = round(_x);
        const u8g2_uint_t y = round(_y);
        u8g2.drawGlyph(x, y, encoding);
        u8g2.setFont(DEFAULT_FONT_MODE);
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

    namespace messages {
        const static char game_over_1[] = {"GAME"};
        const static char game_over_2[] = {"OVER"};
    }


    namespace sprites
    {
        static const byte_t person_run_frames[4][12] = {
        {           // Running 1
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
        {           // Running 2
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
        },
        {           // Jump
            B00000000,
            B00111000,
            B00111000,
            B00111000,
            B10010010,
            B01111100,
            B00111000,
            B00111000,
            B00110010,
            B01101100,
            B10000000,
            B00000000,
        },
        {           // Duck
            B00000000,
            B00000000,
            B00000000,
            B00000000,
            B00111000,
            B01111100,
            B01111100,
            B01010100,
            B00111000,
            B00111000,
            B00101000,
            B00101000,
        }
        };

        static const byte_t reward_frames[32] = {
            // B00000001, B10000000,
            // B00000010, B01000000,
            // B00000010, B01000000,
            // B00000100, B00100000,
            // B11111100, B00111111,
            // B10000000, B00000001,
            // B01000010, B01000010,
            // B00100010, B01000100,
            // B00010010, B01001000,
            // B00010000, B00001000,
            // B00100000, B00000100,
            // B00100000, B00000100,
            // B01000001, B10000010,
            // B01000110, B01100010,
            // B10011000, B00011001,
            // B11100000, B00000111,
            B00000001, B10000000,
            B00000010, B01000000,
            B00000010, B01000000,
            B00000100, B00100000,
            B11111100, B00111111,
            B10000000, B00000001,
            B01000010, B01000010,
            B00100010, B01000100,
            B00010010, B01001000,
            B00010000, B00001000,
            B00100000, B00000100,
            B00100000, B00000100,
            B01000001, B10000010,
            B01000110, B01100010,
            B10011000, B00011001,
            B11100000, B00000111,
        }; // 16x16

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
            end--;
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
        #define MAX_REORDER_BUFFER_SIZE 32
        inline void mitosis_anaphase_reorder(byte_t array[], byte_t n)
        {
            byte_t buffer[MAX_REORDER_BUFFER_SIZE];
            byte_t next_index = 0;
            for (byte_t i = 0; i < n; i += 2) {
                buffer[next_index] = array[i];
                ++next_index;
            }
            for (byte_t j = 1; j < n; j += 2) {
                buffer[next_index] = array[j];
                ++next_index;
            }
            for (byte_t k = 0; k < n; ++k) {
                array[k] = buffer[k];
            }
        }
        inline void inverse_mitosis_anaphase_reorder(byte_t array[], byte_t n)
        {
            byte_t temp;
            for (byte_t i = 0; i < n-1; ++i) {
                temp = i;
            }
        }
        inline void format_sprites()
        {
            for (unit8_t i = 0; i < 4; ++i) {
                reverse_array(person_run_frames[i], 0, 12);
                reverse_all_bytes(person_run_frames[i], 12);
            }

            // mitosis_anaphase_reorder(reward_frames, 32);
            // reverse_array(reward_frames, 0, 32);
            // mitosis_anaphase_reorder(reward_frames, 32);
            reverse_array(reward_frames, 0, 32);
            reverse_all_bytes(reward_frames, 32);
            
            // reverse_array(person_run_frames[0], 0, 12);
            // reverse_all_bytes(person_run_frames[0], 12);
            // reverse_array(person_run_frames[1], 0, 12);
            // reverse_all_bytes(person_run_frames[1], 12);
            // reverse_array(person_run_frames[2], 0, 12);
            // reverse_all_bytes(person_run_frames[2], 12);
            // reverse_array(person_run_frames[3], 0, 12);
            // reverse_all_bytes(person_run_frames[3], 12);
        }
    }
}

#endif