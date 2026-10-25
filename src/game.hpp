#ifndef GAME_HPP
#define GAME_HPP

#include "raylib.h"
#include "level.hpp"
#include <string>
#include <cstdint>

namespace Game {

    struct GameConfig {
        uint32_t window_width;
        uint32_t window_height;
        std::string window_title;

        unsigned int fps;
        float fov;
    };

    class Game {
        public:
            Game();
            ~Game();

            void run();

            uint32_t get_window_width() { return game_config.window_width; }
            uint32_t get_window_height() { return game_config.window_height; }

        private:
            void game_render();

        private:
            GameConfig game_config;
            Level level;

            void loop();
            void on_update();
            void on_render();

    };

}
#endif // GAME_HPP


