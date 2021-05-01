//
// Created by user on 27.04.2021.
//

#ifndef STUD_GAME_GAMEMAP_H
#define STUD_GAME_GAMEMAP_H

#include <string>
#include <utility>
#include <map>
#include <set>
#include <vector>
#include "SFML/Graphics.hpp"

#include "../../DataTypes/index.h"

using namespace std;
using namespace data_types;

namespace engine {
    struct Paddings {
        int left;
        int top;
    };

    class GameMap {
        using t_draw_cb = function<void(size_t y, size_t x, char cell, GameMap& map)>;
        using t_draw_el_cb = function<void(sf::RectangleShape*)>;

        using t_draw_el = pair<sf::RectangleShape*, t_draw_el_cb>;

        public:
            enum Axis: int8_t {
                X,
                Y
            };

            enum Direction: int8_t {
              Top,
              Bottom,
              Left,
              Right
            };

        private:
            const string *m_tile { nullptr };
            size_t m_tile_width;
            size_t m_tile_height;

            size_t m_block_size = 0;
            size_t m_max_block_vising_x = 0;
            size_t m_max_block_vising_y = 0;

            float m_offset_x = 0;
            float m_offset_y = 0;

            Paddings m_paddings {};
            sf::Vector2u m_window_size {0, 0 };

            set<char> m_collision_cells {};
            map<char, t_draw_el> m_cell_draw_el {};
            vector<t_draw_cb> m_on_draw_callbacks {};

        public:
            GameMap(size_t block_size);
            ~GameMap() = default;

            void calculate_offset(const sf::Vector2f& pos);
            void calculate_offset_x(const sf::Vector2f& pos);
            void calculate_offset_y(const sf::Vector2f& pos);

            void draw(sf::RenderWindow& window, pair<size_t, size_t>* player_curr_b = nullptr);
            size_t on_draw(const t_draw_cb& cb, const t_draw_cb& after_cb);
            bool remove_on_draw(size_t pos);

            bool load_tile(const string *tile, size_t width, size_t height);

            bool register_collision_cell(char cell);
            bool register_collision_cells(string str);

            bool register_draw_element(char cell, sf::RectangleShape& rec, const t_draw_el_cb& cb);

            bool set_windows_size(const sf::Vector2u& size);
            bool set_paddings(const Paddings& paddings);

            bool set_max_block_vising_x(int blocks);
            bool set_max_block_vising_y(int blocks);

            bool set_offset_x(float x);
            bool set_offset_y(float y);

            [[nodiscard]] ExtendedRange find_cell_sequence(const pair<size_t, size_t>& start_block, Axis axis) const;

            [[nodiscard]] char at_tile(size_t y, size_t x) const;

            [[nodiscard]] Range get_collision_blocks_x(const sf::Vector2f& pos, const sf::FloatRect& bounds) const;
            [[nodiscard]] Range get_collision_blocks_y(const sf::Vector2f& pos, const sf::FloatRect& bounds) const;

            [[nodiscard]] float get_hitting_in_texture(Direction direction, const sf::Vector2f& pos, const sf::FloatRect& bounds, size_t block) const;

            [[nodiscard]] pair<size_t, size_t> get_current_block(const sf::Vector2f& pos, const sf::FloatRect& bounds) const;
            [[nodiscard]] pair<size_t, size_t> get_adjacent_block(Direction dir, const sf::Vector2f& pos, const sf::FloatRect& bounds) const;

            [[nodiscard]] map<Direction, pair<size_t, size_t>> get_around_blocks(const sf::Vector2f& pos, const sf::FloatRect& bounds) const;

            [[nodiscard]] size_t get_width() const;
            [[nodiscard]] size_t get_height() const;

            [[nodiscard]] size_t get_block_size() const;
            [[nodiscard]] size_t get_max_block_vising_x() const;
            [[nodiscard]] size_t get_max_block_vising_y() const;

            [[nodiscard]] bool is_collision_cell(char cell) const;

            [[nodiscard]] float get_offset_x() const;
            [[nodiscard]] float get_offset_y() const;

            [[nodiscard]] const t_draw_el* get_draw_element(char cell) const;

            [[nodiscard]] const Paddings& get_paddings() const;
            [[nodiscard]] const set<char>& get_collision_cells() const;
    };

}

/**
 *      1. Привязка скорости персонажа ко времени
 *          (S = V*t) => F() Расстояние
 *
 *          S - это путь (расстояние)
 *          V - скорость
 *          t - время
 *
 *          !! Изначально скорость привязана к мощности процессора.
 *          !! Каждое изменение положения (к примеру sprite.move(0.1, 0)) будет происходить каждый такт процессора.
 *      2. Ускорение
 *          (V = V0 + а*t) => F() Скорость тела
 *
 *          V0 - начальная скорость
 *          а - ускорение
 *          t - время
 *
 *      3. Скроллинг карты
 *          - Персонаж не перемещаеться
 *          - Сдвигаеться карта в обратную сторону
 * */

#endif //STUD_GAME_GAMEMAP_H
