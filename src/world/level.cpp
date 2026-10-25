
#include "raylib.h"
#include "level.hpp"

namespace Game {

    Level::Level() {
        camera.position = Vector3{ 0.0f, 5.0f, 5.0f };
        camera.target = Vector3{ 0.0f, 0.0f, 0.0f };
        camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
        camera.fovy = 45.0f;
        camera.projection = CAMERA_PERSPECTIVE;
    }

    Level::~Level() {
    }

    void Level::on_update() {
        UpdateCamera(&camera, CAMERA_FREE);
    }

    void Level::on_render() {
        BeginMode3D(camera);
        {
            DrawCube((Vector3){ -2.0f, 1.0f, -0.0f }, 2.0f, 2.0f, 2.0f, RED);
            DrawCube((Vector3){  2.0f, 1.0f, -5.0f }, 2.0f, 2.0f, 2.0f, RED);
            DrawCube((Vector3){ -2.0f, 1.0f,  5.0f }, 2.0f, 2.0f, 2.0f, RED);
            DrawCube((Vector3){  2.0f, 1.0f,  5.0f }, 2.0f, 2.0f, 2.0f, RED);

            DrawSphere(Vector3{0, 0, 0}, 0.1f, BLUE);
            DrawGrid(10, 1.0f); // Draws a grid on the ground plane

        }
        EndMode3D();
    }

}

