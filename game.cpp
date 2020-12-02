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
    for (byte_t i = 0; i < MAX_REWARDS; ++i) {
        scene_reward_tokens[i] = game::Reward();
        const byte_t offset_index = MAX_OBSTACLES + i;
        scene_objects[offset_index] = &scene_reward_tokens[i];
    }
    player = game::Player();
    scene_objects[TOTAL_GAME_OBJECTS - 1] = &player;
}

void game::SceneController::game_over()
{
    // TODO: render some animation and give controll back to UI
    game::graphics::draw_string(120, 60, "GAME");
    game::graphics::draw_string(120, 40, "OVER");

}

void game::SceneController::start_scene()
{
    game_is_over = false;
    last_step_time = millis();
    for (byte_t i = 0; i < TOTAL_GAME_OBJECTS; ++i) {
        scene_objects[i]->start();
    }
}

void game::SceneController::step_scene()
{
    timestamp_t current_time = millis();
    game::game_state::life_time = current_time;
    timestamp_t l_dt = current_time - last_step_time;
    float delta_time = (float)l_dt / 1000.0; // I want those seconds

    if (game_is_over) {
        if (next_game_schedule < current_time) {
            start_scene();
            return;
        }
        last_step_time = current_time; // Still update delta time. Things are still rendered at game over.
        return;
    }

    for (byte_t i = 0; i < TOTAL_GAME_OBJECTS; ++i) {
        scene_objects[i]->physics_update(delta_time);
    }

    for (byte_t i = 0; i < MAX_OBSTACLES; ++i) {
        if (game::objects_intersecting(&player, &scene_obstacles[i])) {
            game_is_over = true;

            constexpr timestamp_t game_over_delay_seconds = 5;
            constexpr timestamp_t game_over_delay = game_over_delay_seconds * 1000;
            next_game_schedule = current_time + game_over_delay;
            break;
        }
    }

    for (byte_t i = 0; i < MAX_REWARDS; ++i) {
        if (game::objects_intersecting(&player, &scene_reward_tokens[i])) {
            scene_reward_tokens[i].player_gotcha();
            ++player.score;
        }
    }

    if (next_mini_display_render < current_time) {
        game::graphics::mini_display.clearDisplay();
        game::graphics::mini_display.setCursor(10,0);
        game::graphics::mini_display.setTextSize(2);
        game::graphics::mini_display.setTextWrap(false);
        game::graphics::mini_display.setTextColor(SSD1306_WHITE);
        game::graphics::mini_display.println(F(""));
        game::graphics::mini_display.print(F("Score: "));
        game::graphics::mini_display.print(player.score);
        game::graphics::draw_mini_display();
        next_mini_display_render = current_time + 200;
    }

    last_step_time = current_time;
}

void game::SceneController::render()
{
    for (byte_t i = 0; i < TOTAL_GAME_OBJECTS; ++i) {
        scene_objects[i]->render();
    }
    if (game_is_over) {
        game_over();
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
    static game::RelativeCollider<float> a;
        a.top    = obj_a->pos.y + obj_a->collider.top;
        a.bottom = obj_a->pos.y + obj_a->collider.bottom;
        a.left   = obj_a->pos.x + obj_a->collider.left;
        a.right  = obj_a->pos.x + obj_a->collider.right;
    static game::RelativeCollider<float> b;
        b.top    = obj_b->pos.y + obj_b->collider.top;
        b.bottom = obj_b->pos.y + obj_b->collider.bottom;
        b.left   = obj_b->pos.x + obj_b->collider.left;
        b.right  = obj_b->pos.x + obj_b->collider.right;   

    bool disjoint = a.top < b.bottom
        || b.top < a.bottom
        || a.right > b.left
        || b.right > a.left;
    
    return !disjoint;
}

void game::Player::start()
{
    collider.top    =  12.0;
    collider.bottom =  2.0;
    collider.right  =  2.0;
    collider.left   =  5.0;
    pos.x = 120.0;
    pos.y = 0.0;

    action_markov = jogging;

    score = 0;
}

void game::Player::physics_update(const float &delta_time)
{
    Vector2<float> gravity_force = Vector2<float>(0.0, -25.0);

    // if (action_markov == jogging) {
    //     if (game::user_input::jump) {
    //         jump();
    //         game::user_input::jump = false;
    //     } else if (game::user_input::squat) {
    //         squat();
    //         // game::user_input::squat = false;
    //     }
    // } else if (action_markov == ducking) {
    //     if (!game::user_input::squat) {
    //         run();
    //     }
    // }

    if (game::user_input::jump) {
        // Markov edge (e) solution
        if (game::user_input::squat) {
            game::user_input::squat = false;
            run();
        }

        // Normal jump functionality
        jump();
        game::user_input::jump = false;
    } else if (action_markov != airborn && game::user_input::squat) {
        squat();
    } else if (action_markov != airborn && action_markov != jogging) {
        run();
    }

    pos += vel * delta_time;

    if (pos.y > 0.2) {
        // Apply gravity
        vel += gravity_force * delta_time;
    } else if (pos.y < 0.0) {
        run();
    }

    constexpr float max_axis_movement_speed = 20.0;
    // vel.y = constrain(vel.y, -max_axis_movement_speed, max_axis_movement_speed);
    // vel.x = constrain(vel.x, -max_axis_movement_speed, max_axis_movement_speed);
}

noinline void game::Player::run()
{
    vel.y = 0.0;
    pos.y = 0.0;
    collider.top = 12.0;
    action_markov = jogging;
}

void game::Player::jump()
{
    Vector2<float> jump_force = Vector2<float>(0.0, 20.0);

    if (action_markov == jogging) {
        vel += jump_force * 2;
        action_markov = airborn;
    }
}

void game::Player::squat()
{
    if (action_markov == jogging) {
        collider.top = 10.0;
        action_markov = ducking;
    }
}

void game::Player::render()
{
    // Execute the render calls

    constexpr float animation_durration_seconds = 0.2;
    constexpr unsigned long animation_durration_millis = (unsigned long)(1000.0 * animation_durration_seconds);

    if (action_markov == airborn) {
        animation_frame_selector = 2;
    } else if (action_markov == ducking) {
        animation_frame_selector = 3;
    } else if (action_markov == jogging) {
        if (animation_schedule < game::game_state::life_time) {
            ++animation_frame_selector;
            animation_frame_selector %= 2;
            animation_schedule = game::game_state::life_time + animation_durration_millis;
        }
    }

    game::graphics::draw_bitmap(pos.x - 3.5, pos.y, 1, 12, game::graphics::sprites::person_run_frames[animation_frame_selector]);
}


void game::Obstacle::start()
{
    collider.top    =  5.5;
    collider.bottom =  0.0;
    collider.right  = -5.5;
    collider.left   =  5.5;


    respawn();

    constexpr float obstacle_speed = 30.0;
    vel.x = obstacle_speed;
}

void game::Obstacle::physics_update(const float &delta_time)
{
    if (pos.x > 150.0) {
        respawn();
    }
    vel.y += sin(pos.x * 8.) * 2.0;
    pos += vel * delta_time;
}

void game::Obstacle::respawn()
{
    pos.y = (float)random(6, 70);
    pos.x = (float)random(-64, 0);
    vel.y = 0.;
}

void game::Obstacle::render() 
{
    // Execute the render calls
    // https://github.com/olikraus/u8g2/wiki/fntgrpx11#cursor
    constexpr uint16_t box_glyph_addy = 0x50;
    game::graphics::draw_glyph(pos.x - 6.0f, pos.y + 6.0f, box_glyph_addy); // Glyphs draw differently
}

void game::Reward::start()
{
    collider.top    =  8.5;
    collider.bottom =  0.0;
    collider.right  = -8.5;
    collider.left   =  8.5;

    pos.y = 10;
    pos.x = 10;
    vel.x = 20.0;
    is_passing = true;
    respawn();
}

void game::Reward::physics_update(const float &delta_time)
{
    // if (!is_passing) { return; }
    
    if (pos.x > 150.0) {
        respawn();
    }

    pos += vel * delta_time;
}

void game::Reward::respawn()
{
    pos.y = (float)random(6, 70);
    pos.x = (float)random(-64, 0);
    vel.y = 0.;
}

void game::Reward::player_gotcha()
{
    game::game_state::score += 1;
    respawn();
}

void game::Reward::render()
{
    game::graphics::draw_bitmap(pos.x - 8.0f, pos.y - 8.0f, 2, 16, game::graphics::sprites::reward_frames);
}
