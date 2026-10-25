
#include "raylib.h"
#include "game.hpp"
#include <string>
#include <print>

namespace Game {

    Game::Game() {
        int scale = 60;
        game_config.window_width = scale*16;
        game_config.window_height = scale*10;
        game_config.window_title = "igi made in india";

        game_config.fps = 120;
    }

    Game::~Game() {
    }

    void Game::run() {
        InitWindow(game_config.window_width, game_config.window_height, game_config.window_title.c_str());
        SetTargetFPS(game_config.fps);

        // font texture smoothing
        // or else the font is distorted
        SetTextureFilter(GetFontDefault().texture, TEXTURE_FILTER_BILINEAR);

        DisableCursor();
        PollInputEvents();

        while (!WindowShouldClose()) {
            if (IsKeyPressed(KEY_ESCAPE)) {
                if (IsCursorHidden()) {
                    EnableCursor();
                } else {
                    DisableCursor();
                }
            }
            loop();
        }

        CloseWindow();
    }

    void Game::loop() {
        on_update();
        on_render();
    }

    void Game::on_update() {
        level.on_update();
    }

    void Game::on_render() {
        BeginDrawing();
        ClearBackground(BLACK);
        SetExitKey(KEY_NULL);

        game_render();

        // custom fps counter
        int fps_val = GetFPS();
        std::string fps = "FPS: " + std::to_string(fps_val);
        Color text_color = Color{255, 255, 255, 255};
        DrawText(fps.c_str(), 10, 10, 15, text_color);

        EndDrawing();
    }

    void Game::game_render() {
        level.on_render();
    }


}

