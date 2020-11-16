// ---------------------------------------------
// Created by Iain Moncrief on November 13, 2020
// Arduino Game - game.cpp
// ---------------------------------------------

#include "game.h"

game::GameObject::GameObject() {
    pos.x = -1; pos.y = -1;
    // vel.x = 0; vel.y = 0;
}

game::SceneController::SceneController() {
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        scene_obstacles[i] = game::Obstacle();
        scene_objects[i] = &scene_obstacles[i];
    }
    player = game::Player();
    scene_objects[TOTAL_GAME_OBJECTS - 1] = &player;
}

bool game::SceneController::player_did_collide() {
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (player.pos == scene_objects[i]->pos) {
            return true;
        }
    }
    return false;
}

void game::SceneController::game_over() {
    // TODO: render some animation and give controll back to UI
    while (true) {void;}
}

void game::SceneController::step_scene() {

    unsigned long current_time = millis();
    unsigned long l_dt = current_time - last_step_time;
    float delta_time = (float)l_dt / 1000.0; // I want those seconds

    for (int i = 0; i < TOTAL_GAME_OBJECTS; i++) {
        scene_objects[i]->physics_update(delta_time);
    }

    last_step_time = current_time;
}

float abs(const float &x) {
    if (x < 0.0) { return x * -1.0; }
    return x;
}

void game::Player::physics_update(const float &delta_time)
{
    Vector2<float> gravity_force = Vector2<float>(0.0, -0.8);
    Vector2<float> jump_force    = Vector2<float>(0.0, 2.0);

    Vector2<float> ground = Vector2<float>(0.0, 0.0);

    pos += vel * delta_time;
    vel += gravity_force * delta_time;
    if (game::user_input::did_jump()) { // This should happen in the scene controller
        vel += jump_force;
    }

    // This the velocity to zero when the distance between player and the ground approches zero.
    Vector2<float> ground_diff = ground - pos;
    const float ground_diff_threshold = 0.15;
    if (abs(ground_diff.y) > ground_diff_threshold) {
        vel.y = 0;
        pos.y = 0;
    }
}

void setup() {

}