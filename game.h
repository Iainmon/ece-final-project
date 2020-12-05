#ifndef GAME_H
#define GAME_H

#define DEBUG_MODE 1

using timestamp_t = unsigned long;

#ifndef ARDUINO_H
timestamp_t millis();
long random(long, long);
long random();
typedef unsigned char byte;
typedef unsigned char unit8_t;
#endif

typedef unsigned char byte_t;

#define noinline __attribute__((noinline)) \

namespace game {

    inline float frandom() { return random(1, 500) / 100.0; }
    
    namespace user_input {
        void did_jump_interrupt();
        void did_squat_interrupt();
        volatile static bool jump = false;
        volatile static bool squat = false;
    }

    namespace graphics {
        #include "graphics.h"
        using namespace graphics_implementation;
    }

    namespace game_state {
        volatile static timestamp_t life_time;
        volatile static byte_t score;
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
        enum StateMarkov {jogging, airborn, ducking} action_markov;

        byte_t score;

        timestamp_t animation_schedule;
        uint8_t animation_frame_selector;
        inline void jump();
        inline void squat();
        inline void run();

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

    struct Reward : public GameObject
    {
        bool is_passing;
        void start() override;
        void physics_update(const float &delta_time) override;
        void render() override;
        void respawn();
        void player_gotcha();
    };
    

    bool objects_intersecting(GameObject* obj_a, GameObject* obj_b);

    #define MAX_OBSTACLES 4
    #define MAX_REWARDS 3
    #define TOTAL_GAME_OBJECTS (MAX_OBSTACLES+MAX_REWARDS+1)

    class SceneController
    {
        Player player;
        Obstacle scene_obstacles[MAX_OBSTACLES];
        Reward scene_reward_tokens[MAX_REWARDS];
        GameObject* scene_objects[TOTAL_GAME_OBJECTS];
        unsigned long last_step_time;
        void game_over();
        bool game_is_over = false;
        timestamp_t next_game_schedule;
        timestamp_t next_mini_display_render;
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