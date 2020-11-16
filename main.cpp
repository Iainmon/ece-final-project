// ---------------------------------------------
// Created by Iain Moncrief on November 13, 2020
// Arduino Game - main.cpp
// ---------------------------------------------



namespace game {

    #ifndef ARDUINO_H
    typedef unsigned char byte;
    #endif
    typedef byte* t_sprite;
    typedef t_sprite** t_sprite_render_map;
    typedef void (*t_impure_render_routine)(t_sprite_render_map);

    class gamestate;
    class screen;


    struct t_scene_objects
    {
        t_sprite character;
        int x_pos;
        int y_pos;
    };

    #define MAX_GAME_SCENE_OBJECTS 10

    struct t_game_information
    {
        int score;
        int health;
        int time;
        int difficulty;
        t_scene_objects* game_scene[MAX_GAME_SCENE_OBJECTS];
    };

    class gamestate {
        private:
        t_game_information game_information;
        void logic();
        public:
        gamestate();
        gamestate* jumped(const bool&);
        gamestate* squatted(const bool&);
        gamestate* synchronize(const long&);
        gamestate* update_loop(void); gamestate* operator++(int);
        gamestate* operator<<(const gamestate*); // Runs logic
        t_game_information* _impure_inject_game_information(const screen*);
    };

    #define RENDER_ROUTINE_COUNT 2

    class screen {
        private:
        t_impure_render_routine* routines[RENDER_ROUTINE_COUNT];
        t_game_information* _gamestate_game_information;
        void render();
        public:
        screen();
        screen* operator<<(const gamestate*);

        void _attach_impure_render_call(t_impure_render_routine);
        void _set_game_information(const t_game_information*);
    };
}

game::gamestate::gamestate() {
    void;
}
game::gamestate* game::gamestate::operator++(int) {
    this->update_loop();
    return this;
}
game::gamestate* game::gamestate::operator<<(const gamestate* _gs) {
    logic();
    return this;
}

void setup() {

}

game::gamestate* gs = &(game::gamestate());
game::screen* screen = &(game::screen());


void loop() {
    game::gamestate* gs = &(game::gamestate());
    game::screen* screen = &(game::screen());
    game::gamestate* nextGamestate = gs;
    // Read buttons and mutate states.
    (*screen << 
        (*gs << 
            (nextGamestate
                ->jumped(did_squat)
                ->squatted(did_jump)
                ->synchronize(millis())
                ->update_loop()
            )
        )
    );
    

}




#ifndef ARDUINO_H
int main() {
    setup();
    while (true) { loop(); }
}
#endif