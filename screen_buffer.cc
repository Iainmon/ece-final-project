typedef unsigned char t_byte;

union bitbyte
{
    t_byte byte;

    struct
    {
        bool b1 : 1;
        bool b2 : 1;
        bool b3 : 1;
        bool b4 : 1;
        bool b5 : 1;
        bool b6 : 1;
        bool b7 : 1;
        bool b8 : 1;
    };
};

#define WIDTH 12
#define HEIGHT 32
static bitbyte screen_buffer[WIDTH][HEIGHT];

void draw_sprite(t_byte* sprite, const int& sprite_width, const int& sprite_height, const int& x_pos, const int& y_pos) {
    for (int i = 0; i < sprite_width; i++) {
        for (int j = 0; j < sprite_height; j++) {
            bitbyte sprite_byte = (bitbyte)sprite[]
        }
    }
}