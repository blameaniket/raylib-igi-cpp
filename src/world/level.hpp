#ifndef LEVEL_HPP
#define LEVEL_HPP

namespace Game {

    class Level {
        public:
            Level();
            ~Level();

            void on_update();
            void on_render();

        private:
            ::Camera3D camera;
    };

}

#endif // LEVEL_HPP


