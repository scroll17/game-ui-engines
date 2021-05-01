//
// Created by user on 28.04.2021.
//

#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#include <array>
#include <cmath>
#include "pugixml.hpp"

#include "./DataTypes/index.h"
#include "./Engine/index.h"


using json = nlohmann::json;

using namespace std;
using namespace sf;
using namespace form::types;


///
const int H = 14;  /// Высота
const int W = 40;  /// Длина


/////// Каждая ячейка это квадрат 32*32
const string TileMap[H] = {
"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
"B                                B     B",
"B                                B     B",
"B                                B     B",
"B          ZZZ                   B     B",
"B          Z                     B     B",
"B          Z                  BBBB     B",
"BBBB                             B     B",
"BBBB                             B     B",
"B              BB                BB    B",
"B              BB                      B",
"B              BB                      B",
"B    B         BB         BB           B",
"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
};

using namespace engine;

class Player1: public Player {
    public:
        explicit Player1(const GameMap& map): Player(map) {};
        ~Player1() = default;

        void init() override {
            m_position = Vector2f(m_game_map.get_block_size() * 3, m_game_map.get_block_size() * 3);

            m_rect_x = IntRect(16, 225, 85, 85);
            m_rect_y = IntRect(20, 5, 90, 95);

            const auto& paddings = m_game_map.get_paddings();

            m_texture.loadFromFile("/home/user/Code/stud-game/data/textures/user3.png");

            m_sprite.setTexture(m_texture);
            m_sprite.setTextureRect(m_rect_x);
            m_sprite.setPosition({
                m_position.x - m_game_map.get_offset_x() + paddings.left,
                m_position.y - m_game_map.get_offset_y() + paddings.top
            });
            m_sprite.scale(0.3, 0.3);

            m_speed = 0.1;
            m_boost = 0.01;
            m_animation_boost = 0.0015;

            m_dx = m_dy = 0;

            m_dx_current_frame = 1;
            m_dy_current_frame = 0;

            m_max_animation_frames = 4;

            m_animation_change_rate = 0.0035;
            m_run = false;
        }
};

int main() {
    FileReader f_r("./data/json/level_1.json");
    cout << "name => " << f_r.to_json()["name"] << endl;

    const int BLOCK_SIZE = 14;
    const int MINI_BLOCK_SIZE = 4;
    const int MAX_BLOCK_VISING_X = 24;

    const int WINDOW_W = 600;
    const int WINDOW_H = 400;

    int def_offset_x = ((W * BLOCK_SIZE) / 2);
    int def_offset_y = ((H * BLOCK_SIZE) / 2);

    Paddings PADDINGS {
      0,
      (WINDOW_H / 2) - def_offset_y
    };

    RenderWindow window( VideoMode(WINDOW_W, WINDOW_H), "Test!");

    form::types::Text text { "Click [x] to open door" };
    text
      .set_text_size(22)
      .set_window_size(window.getSize())
      .correct_position(true)
      .to_center(Element::XY)
      .move(Element::Y, WINDOW_H / 3)
      .build();

    GameMap map(BLOCK_SIZE);
    map.set_windows_size(window.getSize());
    map.load_tile(TileMap, W, H);
    map.set_paddings(PADDINGS);
    map.register_collision_cells("BZ");

    GameMap mini_map(MINI_BLOCK_SIZE);
    mini_map.set_windows_size(window.getSize());
    mini_map.load_tile(TileMap, W, H);
    mini_map.set_max_block_vising_x(MAX_BLOCK_VISING_X);
    mini_map.set_max_block_vising_y(MAX_BLOCK_VISING_X);
    mini_map.set_paddings({ 5, 5 });

    Player1 player1(map);
    player1.init();

    RectangleShape rectangle(Vector2f(BLOCK_SIZE, BLOCK_SIZE));
    rectangle.setFillColor(Color::White);

    RectangleShape mini_rectangle(Vector2f(MINI_BLOCK_SIZE, MINI_BLOCK_SIZE));

    mini_map.register_draw_element(' ', mini_rectangle, [](sf::RectangleShape* rect) -> void {
        rect->setFillColor(sf::Color::White);
    });
    mini_map.register_draw_element('B', mini_rectangle, [](sf::RectangleShape* rect) -> void {
        rect->setFillColor(sf::Color::Green);
    });
    mini_map.register_draw_element('Z', mini_rectangle, [](sf::RectangleShape* rect) -> void {
        rect->setFillColor(sf::Color::Yellow);
    });

    map.register_draw_element(' ', rectangle, [](sf::RectangleShape* rect) -> void {
        rect->setFillColor(sf::Color::White);
    });
    map.register_draw_element('B', rectangle, [](sf::RectangleShape* rect) -> void {
        rect->setFillColor(sf::Color::Black);
    });
    map.register_draw_element('Z', rectangle, [](sf::RectangleShape* rect) -> void {
        rect->setFillColor(sf::Color::Yellow);
    });

    mini_map.on_draw(
      [&player1, &mini_rectangle, &map](size_t y, size_t x, char cell, GameMap& mini_map) {
          auto player_on_map = map.get_current_block(player1.get_position(), player1.get_bounds());
          int player_x = player_on_map.first;
          int player_y = player_on_map.second;


          if(player_y == y && player_x == x) {
              mini_rectangle.setFillColor(Color::Red);
          }
      },
      [](size_t y, size_t x, char cell, GameMap& map) {}
    );

    bool show_text = false;

    Clock clock;
    while (window.isOpen()) {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();

        time = time / 600;

        show_text = false;

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();

            player1.run(event.key.shift);
        }

        Player::move_input(player1);

        player1.update(time);
        player1.around_blocks('Z', [&](auto& block) -> void {
            auto door_key = map
                    .find_cell_sequence(block, GameMap::Axis::X)
                    .to_string([](const string& iter, const string& second_val) {
                        string str {};

                        str.append("[").append(iter).append(",").append(second_val).append("]");

                        return str;
                    });

            cout << "door = " <<  f_r.to_json()["doors"][door_key] << endl;

            show_text = true;
        });

        window.clear(Color::Yellow);

        map.calculate_offset(player1.get_position());

        auto player_on_map = map.get_current_block(player1.get_position(), player1.get_bounds());

        map.draw(window);
        mini_map.draw(window, &player_on_map);

        if(show_text) {
            text.draw(window);
        }

        window.draw(player1.get_sprite());
        window.display();
    }

    return EXIT_SUCCESS;
}