https://www.kjeld-schmidt.com/pixel-editor-for-arduino-oled-i2c-display/
https://www.elithecomputerguy.com/2019/07/draw-shapes-on-oled-screen-with-arduino/
https://learn.adafruit.com/adafruit-gfx-graphics-library

Possible solution for the bitmap problem.
```cpp
#include <iostream>

union Byte
{
    unsigned char byte;

    struct
    {
        bool bit1 : 1;
        bool bit2 : 1;
        bool bit3 : 1;
        bool bit4 : 1;
        bool bit5 : 1;
        bool bit6 : 1;
        bool bit7 : 1;
        bool bit8 : 1;
    };
};

int main ()
{
    Byte b;

    b.byte = 0x65; // 0110 0101

    std::cout << b.bit8 << b.bit7 << b.bit6 << b.bit5 << ' '
              << b.bit4 << b.bit3 << b.bit2 << b.bit1 ;
}
```

https://forum.arduino.cc/index.php?topic=602954.0
https://wiki.seeedstudio.com/I2C_LCD/
https://github.com/nickgammon/I2C_graphical_LCD_display
https://thesolaruniverse.wordpress.com/2017/08/11/an-128x64-graphic-lcd-display-with-st7920-controller-for-the-arduino-displaying-temperature-and-relative-humidity/
https://adafruit.github.io/Adafruit_SSD1306/html/class_adafruit___s_s_d1306.html#a2661e6166a7a1c00695ed55eaa4ece46

Use a box collider implementation: http://www.cplusplus.com/forum/beginner/167138/