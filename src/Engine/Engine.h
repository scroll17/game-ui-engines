//
// Created by user on 10.12.2020.
//

#ifndef STUD_GAME_ENGINE_H
#define STUD_GAME_ENGINE_H

#include <SFML/Graphics.hpp>

#include "../DataTypes/index.h"
#include "../Constants/Constants.h"

using namespace std;

namespace engine {

    class Engine {
        private:
            static sf::Font GameFont;

        public:
            void start();
            void stop();

            static sf::Font& get_game_font();
    };

}

#endif //STUD_GAME_ENGINE_H
