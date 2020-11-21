#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <U8g2lib.h>

#include "/home/iainmoncrief/Documents/Github/ece-final-project/game.h"
#include "/home/iainmoncrief/Documents/Github/ece-final-project/game.cpp"

#define JUMP_BUTTON 2
#define SQUAT_BUTTON 3

void game::user_input::did_jump_interrupt() {
    game::user_input::jump = true;
}

void game::user_input::did_squat_interrupt() {
    game::user_input::squat = !game::user_input::squat;
}

static game::SceneController game_scene;

#if DEBUG_MODE
game::Player* player = game_scene.expose_player();
#endif

void setup()
{

    game::graphics::sprites::format_sprites();
    game::graphics::initialize_screens();
    game_scene.start_scene();
    // game::graphics::initial_refresh();

    attachInterrupt(digitalPinToInterrupt(JUMP_BUTTON), game::user_input::did_jump_interrupt, RISING);
    attachInterrupt(digitalPinToInterrupt(SQUAT_BUTTON), game::user_input::did_squat_interrupt, CHANGE);
    // attachInterrupt(digitalPinToInterrupt(SQUAT_BUTTON), game::user_input::did_squat_interrupt, FALLING);

}

void loop()
{
    game::graphics::initial_refresh();
    do
    {
        game::graphics::pre_update();
        game_scene.step_scene();
        game_scene.render();

    } while (game::graphics::non_terminating_refresh());

    //     game::graphics::pre_update();
    //     game_scene.step_scene();
    //     game_scene.render();

    // game::graphics::non_terminating_refresh();

}