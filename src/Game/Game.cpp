//
// Created by user on 10.12.2020.
//

#include "Game.h"

void Game::start() {

}

sf::Font Game::GameFont = sf::Font();

sf::Font& Game::get_game_font() {
    auto info = Game::GameFont.getInfo();
    if(info.family.empty()) {
        string path = File::resolve_path(Constants::directory_path, Constants::game_font_path);

        bool res = Game::GameFont.loadFromFile(path);
        if (!res) {
            std::cerr << "Failed load GameFont" << endl;
        }
    }

    return Game::GameFont;
}