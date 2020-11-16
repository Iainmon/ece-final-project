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
