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
using namespace engine;
using namespace form::types;

class Player1: public Player {
    public:
        explicit Player1(const GameMap& map): Player(map) {};
        ~Player1() = default;

        void init() override {
            m_position = Vector2f(m_game_map.get_block_size() * 27, m_game_map.get_block_size() * 10.5);

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
    const json level_data = f_r.to_json();
    const json& level_map = level_data["map"];

    const int max_floor = level_data["floorsCount"];
    int current_floor = 0;

    json current_map = level_map[current_floor];
    string* tail_map = utils::string::json_arr_to_string(current_map["tail"], current_map["height"]);

    const int BLOCK_SIZE = 14;
    const int MINI_BLOCK_SIZE = 4;
    const int MAX_BLOCK_VISING_X = 24;

    const int WINDOW_W = 600;
    const int WINDOW_H = 400;

    Paddings PADDINGS {
      0,
      (WINDOW_H / 2) - ((int(current_map["height"]) * BLOCK_SIZE) / 2)
    };

    RenderWindow window( VideoMode(WINDOW_W, WINDOW_H), "Test!");

    form::types::Text door_text { "Click [x] to open door" };
    door_text
      .set_text_size(22)
      .set_window_size(window.getSize())
      .correct_position(true)
      .to_center(Element::XY)
      .move(Element::Y, WINDOW_H / 3)
      .build();

    form::types::Text floor_text { "Click [z] to change floor" };
    floor_text
      .set_text_size(22)
      .set_window_size(window.getSize())
      .correct_position(true)
      .to_center(Element::XY)
      .move(Element::Y, WINDOW_H / 3)
      .build();

    GameMap map(BLOCK_SIZE);
    map.set_windows_size(window.getSize());
    map.load_tile(tail_map, current_map["width"], current_map["height"]);
    map.set_paddings(PADDINGS);
    map.register_collision_cells("BZD");

    GameMap mini_map(MINI_BLOCK_SIZE);
    mini_map.set_windows_size(window.getSize());
    mini_map.load_tile(tail_map, current_map["width"], current_map["height"]);
    mini_map.set_max_block_vising_x(MAX_BLOCK_VISING_X);
    mini_map.set_paddings({ 5, 5 });

    Player1 player1(map);
    player1.init();

    const auto& reload_current_map = [&map, &mini_map, &level_map, &current_map, &current_floor, &tail_map](int new_floor) {
        current_floor = new_floor;
        current_map = level_map[current_floor];

        delete[] tail_map;
        tail_map = utils::string::json_arr_to_string(current_map["tail"], current_map["height"]);

        map.load_tile(tail_map, current_map["width"], current_map["height"]);
        mini_map.load_tile(tail_map, current_map["width"], current_map["height"]);
    };

    const auto& update_player_pos = [&player1, &map](float x, float y) {
        {
//        auto begin = floor_key.begin();
//        auto end = floor_key.end();
//
//        string str(begin + 1, end - 1);
//        int comma_pos = str.find(',');
//
//        auto x = str.substr(0, comma_pos);
//        auto y = str.substr(comma_pos + 1, str.size() - comma_pos);
//
//        size_t x_block, y_block;
//        if(x.find('-' != -1)) {
//            size_t x_comma_pos = x.find('-');
//            size_t x_start = std::stoi(x.substr(0, x_comma_pos));
//            size_t x_end = std::stoi(x.substr(x_comma_pos + 1, x.size() - x_comma_pos));
//
//            y_block = std::stoi(y);
//            x_block = (x_start + x_end) / 2;
//        } else {
//            size_t y_comma_pos = x.find('-');
//            size_t y_start = std::stoi(y.substr(0, y_comma_pos));
//            size_t y_end = std::stoi(y.substr(y_comma_pos + 1, y.size() - y_comma_pos));
//
//            x_block = std::stoi(x);
//            y_block = (y_start + y_end) / 2;
//        }
        }

        player1.set_position(x * map.get_block_size(), y * map.get_block_size());
        map.calculate_offset(player1.get_position());
    };

    RectangleShape rectangle(Vector2f(BLOCK_SIZE, BLOCK_SIZE));
    rectangle.setFillColor(Color::White);

    RectangleShape mini_rectangle(Vector2f(MINI_BLOCK_SIZE, MINI_BLOCK_SIZE));

    {
        mini_map.register_draw_element(' ', mini_rectangle, [](sf::RectangleShape* rect) -> void {
            rect->setFillColor(sf::Color::White);
        });
        mini_map.register_draw_element('B', mini_rectangle, [](sf::RectangleShape* rect) -> void {
            rect->setFillColor(sf::Color::Green);
        });
        mini_map.register_draw_element('Z', mini_rectangle, [](sf::RectangleShape* rect) -> void {
            rect->setFillColor(sf::Color::Yellow);
        });
    }

    {
        map.register_draw_element(' ', rectangle, [](sf::RectangleShape* rect) -> void {
            rect->setFillColor(sf::Color::White);
        });
        map.register_draw_element('B', rectangle, [](sf::RectangleShape* rect) -> void {
            rect->setFillColor(sf::Color::Black);
        });
        map.register_draw_element('Z', rectangle, [](sf::RectangleShape* rect) -> void {
            rect->setFillColor(sf::Color::Yellow);
        });
        map.register_draw_element('D', rectangle, [](sf::RectangleShape* rect) -> void {
            rect->setFillColor(sf::Color::Cyan);
        });
    }

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

    bool can_change_floor = false;
    bool can_open_door = false;

    auto key_z_pressed = false;

    Clock clock;
    while (window.isOpen()) {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();

        time = time / 600;

        can_open_door = false;
        can_change_floor = false;
        key_z_pressed = false;

        Event event {};
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();

            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z) {
                key_z_pressed = true;
            }

            player1.run(event.key.shift);
        }

        Player::move_input(player1);

        player1.update(time);
        player1.around_blocks('Z', [&](auto& block) -> void {
            auto door_key = map
                    .find_cell_sequence(block, GameMap::Axis::X)
                    .to_string(0);

//            cout << "door_key = " <<  door_key << endl;
//            cout << "door = " <<  level_data["map"][0]["doors"][door_key] << endl;

            can_open_door = true;
        });

        string last_floor_key {};
        player1.around_blocks('D', [&](auto& block) -> void {
            auto last_axis = GameMap::Axis::X;
            auto floor_range = map.find_cell_sequence(block, last_axis);

            if(floor_range.get_start() == floor_range.get_end()) {
                floor_range = map.find_cell_sequence(block, last_axis = GameMap::Axis::Y);
            }

            last_floor_key = floor_range.to_string(last_axis);
            can_change_floor = true;
        });

        window.clear(Color::Yellow);

        map.calculate_offset(player1.get_position());

        auto player_on_map = map.get_current_block(player1.get_position(), player1.get_bounds());

        if(key_z_pressed && can_change_floor) {
            const auto& floor = current_map["floors"][last_floor_key];

            if(!floor.is_null()) {
                int new_floor = floor["val"];
                json new_player_pos = floor["pos"];

                reload_current_map(new_floor);
                update_player_pos(new_player_pos[0], new_player_pos[1]);
            }
        }

        map.draw(window);
        mini_map.draw(window, &player_on_map);

        if(can_open_door) {
            door_text.draw(window);
        }
        if(can_change_floor) {
            floor_text.draw(window);
        }

        window.draw(player1.get_sprite());
        window.display();
    }


    delete[] tail_map;

    return EXIT_SUCCESS;
}