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
    for (byte_t i = 0; i < MAX_OBSTACLES; ++i) {
        scene_obstacles[i] = game::Obstacle();
        scene_objects[i] = &scene_obstacles[i];
    }
    player = game::Player();
    scene_objects[TOTAL_GAME_OBJECTS - 1] = &player;
}

void game::SceneController::game_over()
{
    // TODO: render some animation and give controll back to UI
    game::graphics::draw_string(100, 20, "GAME OVER!");
}

void game::SceneController::start_scene()
{
    last_step_time = millis();
    for (byte_t i = 0; i < TOTAL_GAME_OBJECTS; ++i) {
        scene_objects[i]->start();
    }
}

void game::SceneController::step_scene()
{
    if (game_is_over) {
        game_over();
        return;
    }

    timestamp_t current_time = millis();
    game::game_state::life_time = current_time;
    
    timestamp_t l_dt = current_time - last_step_time;
    float delta_time = (float)l_dt / 1000.0; // I want those seconds

    for (byte_t i = 0; i < TOTAL_GAME_OBJECTS; ++i) {
        scene_objects[i]->physics_update(delta_time);
    }

    for (byte_t i = 0; i < MAX_OBSTACLES; ++i) {
        if (game::objects_intersecting(&player, &scene_obstacles[i])) {
            game_is_over = true;
            break;
        }
    }

    last_step_time = current_time;
}

void game::SceneController::render()
{
    for (byte_t i = 0; i < TOTAL_GAME_OBJECTS; ++i) {
        scene_objects[i]->render();
    }
}

// #ifndef ARDUINO_H
// float abs(const float &x) {
//     if (x < 0.0) { return x * -1.0; }
//     return x;
// }
// #endif

bool game::objects_intersecting(game::GameObject* obj_a, game::GameObject* obj_b)
{
    // b1.top < b2.bottom // b1 below b2
    // || b2.top < b1.bottom // b2 below b1
    // || b1.right < b2.left // b2 to the right of b1
    // || b2.right < b1.left
    game::RelativeCollider<float> a;
        a.top    = obj_a->pos.y + obj_a->collider.top;
        a.bottom = obj_a->pos.y + obj_a->collider.bottom;
        a.left   = obj_a->pos.x + obj_a->collider.left;
        a.right  = obj_a->pos.x + obj_a->collider.right;
    game::RelativeCollider<float> b;
        b.top    = obj_b->pos.y + obj_b->collider.top;
        b.bottom = obj_b->pos.y + obj_b->collider.bottom;
        b.left   = obj_b->pos.x + obj_b->collider.left;
        b.right  = obj_b->pos.x + obj_b->collider.right;   

    bool disjoint = a.top < b.bottom
        || b.top < a.bottom
        || a.right < b.left
        || b.right < a.left;
    
    return !disjoint;
}

void game::Player::start()
{
    collider.top    =  6.0;
    collider.bottom =  0.0;
    collider.right  =  0.1;
    collider.left   = -0.1;
    pos.x = 100.0;
    pos.y = 0.0;
}

void game::Player::physics_update(const float &delta_time)
{
    Vector2<float> gravity_force = Vector2<float>(0.0, -10.0);
    Vector2<float> jump_force    = Vector2<float>(0.0, 6.0);

    Vector2<float> ground = Vector2<float>(0.0, 0.0);

    if (game::user_input::jump) { // This should happen in the scene controller
        vel += jump_force;
        game::user_input::jump = false;
    }

    pos += vel * delta_time;


    if (pos.y > 0.2) {
        vel += gravity_force * delta_time; // gravity_force;
    } else if (pos.y < 0.0) {
        vel.y = 0.0;
        pos.y = 0.0;
    }

    constexpr float max_axis_movement_speed = 20.0;
    vel.y = constrain(vel.y, -max_axis_movement_speed, max_axis_movement_speed);
    // vel.x = constrain(vel.x, -max_axis_movement_speed, max_axis_movement_speed);


    // This the velocity to zero when the distance between player and the ground approches zero.
    // Vector2<float> ground_diff = ground - pos;
    // const float ground_diff_threshold = 0.15;
    // if (abs(ground_diff.y) > ground_diff_threshold) {
    //     vel.y = 0;
    //     pos.y = 0;
    // }
}

void game::Player::render()
{
    // Execute the render calls

    // game::graphics::draw_box(pos.x, pos.y, 3, 3);

    constexpr float animation_durration_seconds = 0.2;
    constexpr unsigned long animation_durration_millis = (unsigned long)(1000.0 * animation_durration_seconds);

    if (animation_schedule < game::game_state::life_time) {
        ++animation_frame_selector;
        animation_frame_selector %= 2;
        animation_schedule = game::game_state::life_time + animation_durration_millis;
    }

    game::graphics::draw_bitmap(pos.x, pos.y, 1, 12, game::graphics::sprites::person_run_frames[animation_frame_selector]);
}


void game::Obstacle::start()
{
    collider.top    =  5.0;
    collider.bottom = -5.0;
    collider.right  =  5.0;
    collider.left   = -5.0;


    pos.y = (float)random(0, 30);
    pos.x = (float)random(-30, 0);

    constexpr float obstacle_speed = 30.0;
    vel.x = obstacle_speed;
}

void game::Obstacle::physics_update(const float &delta_time)
{
    if (pos.x > 150.0) {
        respawn();
    }
    pos += vel * delta_time;
}

void game::Obstacle::respawn()
{
    pos.y = (float)random(0, 64);
    pos.x = (float)random(0, 10);
}

void game::Obstacle::render() 
{
    // Execute the render calls
    constexpr uint16_t snowman_glyph_addy = 0x2603;
    game::graphics::draw_frame(pos.x, pos.y, 8, 8);
    // game::graphics::draw_glyph(pos.x, pos.y, snowman_glyph_addy);
}

