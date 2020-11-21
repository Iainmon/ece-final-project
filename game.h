// ---------------------------------------------
// Created by Iain Moncrief on November 13, 2020
// Arduino Game - game.h
// ---------------------------------------------

#ifndef GAME_H
#define GAME_H

#define DEBUG_MODE 1

#ifndef ARDUINO_H
unsigned long millis();
long random(long, long);
long random();
#endif

namespace game {

    inline float frandom() { return random(1, 500) / 100.0; }
    
    namespace user_input {
        void did_jump_interrupt();
        void did_squat_interrupt();
        volatile static bool jump = false;
        volatile static bool squat = false;
    }

    namespace graphics {
        // Each digit spot is 5x8 binary pixels. 
        // The screen we will use is 20x4 digits.
        // The memory required to store a screen state is 3200 bits (3.2Kb) which is 400 bytes.

        #include "graphics.h"
        using namespace graphics_implementation;
    }

    template<typename T>
    struct RelativeCollider
    {
        T top;
        T bottom;
        T left;
        T right;
        RelativeCollider() { }
        RelativeCollider(T a, T b, T c, T d) : top(a), bottom(b), left(c), right(d) { }
    };

    template<typename T>
    struct Vector2
    {
        T x;
        T y;
        Vector2<T>(const T &_x, const T &_y) : x(_x), y(_y) {}
        Vector2<T>() { x = T(); y = T(); }
        inline bool operator==(const Vector2<T> &rhs) {
            return (x == rhs.x) && (y == rhs.y);
        }
        inline Vector2<T>& operator+=(const Vector2<T> &rhs) { // This may not be allowed to be inline.
            this->x += rhs.x;
            this->y += rhs.y;
            return *this;
        }
        inline Vector2<T> operator+(const Vector2<T> &rhs) {
            Vector2<T> vec;
            vec.x = x + rhs.x;
            vec.y = y + rhs.y;
            return vec;
        }
        inline Vector2<T> operator-(const Vector2<T> &rhs) {
            Vector2<T> vec;
            vec.x = x - rhs.x;
            vec.y = y - rhs.y;
            return vec;
        }
        inline Vector2<T>& operator*=(const T &rhs) {
            this->x *= rhs;
            this->y *= rhs;
            return *this;
        }
        inline Vector2<T> operator*(const T &rhs) {
            Vector2<T> vec;
            vec.x = x * rhs;
            vec.y = y * rhs;
            return vec;
        }
        inline Vector2<T> operator/(const T &rhs) {
            Vector2<T> vec;
            vec.x = x / rhs;
            vec.y = y / rhs;
            return vec;
        }
        inline Vector2<T> operator-() {
            Vector2<T> vec;
            vec.x = -x;
            vec.y = -y;
            return vec;
        }
    };

    struct GameObject
    {
        Vector2<float> pos; // possition
        Vector2<float> vel; // velocity
        RelativeCollider<float> collider;
        GameObject();
        virtual void start() = 0;
        virtual void physics_update(const float &delta_time) = 0;
        virtual void render() = 0;
    };

    struct Player: public GameObject
    {
        void start() override;
        void physics_update(const float &delta_time) override;
        void render() override;
    };

    struct Obstacle : public GameObject
    {
        void start() override;
        void physics_update(const float &delta_time) override;
        void render() override;
        void respawn();
    };

    bool objects_intersecting(GameObject* obj_a, GameObject* obj_b);

    #define MAX_OBSTACLES 6
    #define TOTAL_GAME_OBJECTS MAX_OBSTACLES+1

    class SceneController
    {
        Player player;
        Obstacle scene_obstacles[MAX_OBSTACLES];
        GameObject* scene_objects[MAX_OBSTACLES + 1];
        unsigned long last_step_time;
        void game_over();
        bool game_is_over = false;
        public:
        SceneController();
        void start_scene();
        void step_scene();
        void render();
        #if DEBUG_MODE
        Player* expose_player() { return &player; }
        #endif
    };
}

#endif