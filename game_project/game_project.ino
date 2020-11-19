#include "PATH/ece-final-project/game.h"
#include "PATH/ece-final-project/game.cpp"

#define JUMP_BUTTON 4
#define SQUAT_BUTTON 5

bool game::user_input::did_jump() {
    return digitalRead(JUMP_BUTTON);
}
bool game::user_input::did_squat() {
    return digitalRead(SQUAT_BUTTON);
}

game::SceneController game_scene;

void setup()
{
    game_scene.start_scene();
}

void loop()
{
    game_scene.start_scene();
}