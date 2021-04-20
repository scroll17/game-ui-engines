//
// Created by user on 10.12.2020.
//

#include "Engine.h"

using namespace engine;

void Engine::start() {

}

sf::Font Engine::GameFont = sf::Font();

sf::Font& Engine::get_game_font() {
    auto& info = Engine::GameFont.getInfo();
    if(info.family.empty()) {
        string path = data_types::File::resolve_path(Constants::directory_path, Constants::game_font_path);

        bool res = Engine::GameFont.loadFromFile(path);
        if (!res) {
            std::cerr << "Failed load GameFont" << endl;
        }
    }

    return Engine::GameFont;
}