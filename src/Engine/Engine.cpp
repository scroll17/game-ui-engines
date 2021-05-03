//
// Created by user on 10.12.2020.
//

#include "Engine.h"

using namespace engine;
using namespace sf;

sf::Font Engine::GameFont = sf::Font();

Engine::Engine(RenderWindow& win): window(win) {
}

/// PUBLIC GET
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

/// PUBLIC VOID
void Engine::start() {
    Clock clock;
    while (window.isOpen()) {
        float elapsed_time = clock.getElapsedTime().asMicroseconds();
        clock.restart();

        elapsed_time = elapsed_time / 600;

//        this->input();
//        this->update(dtAsSeconds);
//        this->draw();
    }
}

void Engine::input() {
    Event event {};
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) window.close();
    }

}
