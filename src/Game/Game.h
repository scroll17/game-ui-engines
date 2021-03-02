//
// Created by user on 10.12.2020.
//

#ifndef STUD_GAME_GAME_H
#define STUD_GAME_GAME_H

#include <SFML/Graphics.hpp>

#include "../File/File.h"
#include "../Constants/Constants.h"

using namespace std;

class Game {
    public:
        void start();

        static sf::Font& get_game_font();

    private:
        static sf::Font GameFont;
};

#endif //STUD_GAME_GAME_H
