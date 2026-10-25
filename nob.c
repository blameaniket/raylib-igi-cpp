#define NOB_IMPLEMENTATION
#include "external/nob.h"

#define BUILD_DIR "build"
#define RAYLIB_SRC_DIR "external/raylib/src"
#define RAYLIB_BUILD_DIR BUILD_DIR "/raylib"
#define RAYLIB_LIB_PATH RAYLIB_BUILD_DIR "/libraylib.a"

// Raylib modules to compile.
// Note: rglfw.c is Raylib's GLFW single-file translation unit.
// Compiling rglfw with -D_GLFW_X11 and -DPLATFORM_DESKTOP provides all glfw* symbols.
static const char *raylib_modules[] = {
    "rcore",
    "rshapes",
    "rtextures",
    "rtext",
    "rmodels",
    "raudio",
    "rglfw",
};

bool build_raylib()
{
    if (!mkdir_if_not_exists(BUILD_DIR)) return false;
    if (!mkdir_if_not_exists(RAYLIB_BUILD_DIR)) return false;

    Procs procs = {0};
    Cmd cmd = {0};
    bool any_rebuilt = false;

    const char *obj_paths[NOB_ARRAY_LEN(raylib_modules)];

    for (size_t i = 0; i < NOB_ARRAY_LEN(raylib_modules); ++i) {
        const char *src_path = temp_sprintf("%s/%s.c", RAYLIB_SRC_DIR, raylib_modules[i]);
        const char *obj_path = temp_sprintf("%s/%s.o", RAYLIB_BUILD_DIR, raylib_modules[i]);
        obj_paths[i] = obj_path;

        int rebuild = needs_rebuild1(obj_path, src_path);
        if (rebuild < 0) return false;
        if (rebuild == 1) {
            any_rebuilt = true;
            cmd_append(&cmd, "cc",
                "-O2",
                "-fPIC",
                "-DPLATFORM_DESKTOP",
                "-D_GLFW_X11",
                "-DGRAPHICS_API_OPENGL_33",
                temp_sprintf("-I%s", RAYLIB_SRC_DIR),
                temp_sprintf("-I%s/external/glfw/include", RAYLIB_SRC_DIR),
                "-c", src_path,
                "-o", obj_path
            );
            if (!cmd_run(&cmd, .async = &procs)) return false;
        }
    }

    if (!procs_flush(&procs)) return false;

    int lib_rebuild = any_rebuilt ? 1 : needs_rebuild(RAYLIB_LIB_PATH, obj_paths, NOB_ARRAY_LEN(obj_paths));
    if (lib_rebuild < 0) return false;
    if (lib_rebuild == 1) {
        cmd_append(&cmd, "ar", "rcs", RAYLIB_LIB_PATH);
        for (size_t i = 0; i < NOB_ARRAY_LEN(raylib_modules); ++i) {
            cmd_append(&cmd, obj_paths[i]);
        }
        if (!cmd_run(&cmd)) return false;
    }

    return true;
}

bool build_app(void)
{
    const char *sources[] = {
        "src/main.cpp",
        "src/game.cpp",
        "src/game.hpp",
        RAYLIB_LIB_PATH,
    };

    int rebuild = needs_rebuild(BUILD_DIR "/app", sources, NOB_ARRAY_LEN(sources));
    if (rebuild < 0) return false;
    if (rebuild == 1) {
        Cmd cmd = {0};
        cmd_append(&cmd, "c++",
            "-std=c++23",
            "-Wall", "-Wextra", "-ggdb",
            "-Isrc",
            "-Isrc/world",
            "-Iexternal/glm",
            "-Iexternal/raylib/src",
            "-Iexternal/raylib-cpp/include",
            "src/main.cpp",
            "src/game.cpp",
            "src/world/level.cpp",
            "-o", BUILD_DIR "/app",
            RAYLIB_LIB_PATH,
            "-lGL", "-lm", "-lpthread", "-ldl", "-lrt", "-lX11"
        );
        if (!cmd_run(&cmd)) return false;
    }

    return true;
}

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    const char *program_name = shift_args(&argc, &argv);
    (void)program_name;

    const char *subcommand = NULL;
    if (argc > 0) {
        subcommand = shift_args(&argc, &argv);
    }

    if (subcommand && strcmp(subcommand, "clean") == 0) {
        Cmd cmd = {0};
        cmd_append(&cmd, "rm", "-rf", BUILD_DIR);
        if (!cmd_run(&cmd)) return 1;
        nob_log(NOB_INFO, "Cleaned build directory");
        return 0;
    }

    if (!build_raylib()) return 1;
    if (!build_app()) return 1;

    if (subcommand && strcmp(subcommand, "run") == 0) {
        Cmd cmd = {0};
        cmd_append(&cmd, BUILD_DIR "/app");
        da_append_many(&cmd, argv, argc);
        if (!cmd_run(&cmd)) return 1;
        return 0;
    }

    nob_log(NOB_INFO, "Build successful! Run with ./%s or ./%s run", BUILD_DIR "/app", "nob");
    return 0;
}
