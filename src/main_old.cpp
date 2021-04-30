#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#include "iostream"
#include <map>
#include <array>
#include <cmath>
#include "pugixml.hpp"

#include "./DataTypes/index.h"
#include "./Engine/index.h"


using json = nlohmann::json;

using namespace std;
using namespace sf;


//struct Var_with_default {
//    private:
//        float m_default;
//
//    public:
//        float m_value;
//
//        Var_with_default(float def, float val): m_default(def), m_value(val + def) {};
//
//        // Перегрузка оператора присваивания
//        Var_with_default& operator= (float val)
//        {
//            m_value = val + m_default;
//
//            return *this;
//        }
//
//
//        // Выполняем float + Var_with_default через дружественную функцию
//        friend Var_with_default& operator+(Var_with_default &d1, const float &d2) {
//            d1.m_value += d2;
//            return d1;
//        };
//        friend float operator+(const float &d1, const Var_with_default &d2) {
//          return d1 + (d2.m_value);
//        };
//
//        friend Var_with_default& operator-(Var_with_default &d1, const float &d2) {
//            if((d1.m_value - d2) <= d1.m_default) return d1;
//
//            d1.m_value -= d2;
//            return d1;
//        };
//        friend float operator-(const float &d1, const Var_with_default &d2) {
//            return d1 - (d2.m_value - d2.m_default);
////            return d1 - d2.m_value;
//        };
//};

//const int H = 12 + 8;  /// Высота
//const int W = 40;  /// Длина

///
const int H = 14;  /// Высота
const int W = 73;  /// Длина

const int BLOCK_SIZE = 14; /// Размер блока;
const int MINI_BLOCK_SIZE = 4;

const int MAX_BLOCK_VISING_X = 24;

const int WINDOW_W = 600;
const int WINDOW_H = 400;

struct Paddings {
    int left;
    int top;
};

int def_offset_x = ((W * BLOCK_SIZE) / 2);
int def_offset_y = ((H * BLOCK_SIZE) / 2);

Paddings PADDINGS {
//   (WINDOW_W / 2) - def_offset_x,
0,
   (WINDOW_H / 2) - def_offset_y
};

// class Map
using namespace form::types;
//struct Offset {
//    private:
//        float m_val;
//        Element::Axis m_axis;
//        const Paddings& m_padding {};
//
//    public:
//        Offset(float val, Element::Axis axis, const Paddings& pad): m_val(val), m_axis(axis), m_padding(pad) {};
//        ~Offset() = default;
//
//        [[nodiscard]] float get_value() const {
//            if(m_axis == form::types::Element::Axis::X) {
//                return m_val + m_padding.left;
//            } else {
//                return m_val + m_padding.top;
//            }
//        }
//
//        void set_value(float value) {
//            m_val = value;
//        }
//};

float offsetX = 0;
float offsetY = 0;

int show_text = false;

string TileMap[H] = {
"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
"B                                                                       B",
"B      BB                                                               B",
"B      BB                                                               B",
"B                                                                       B",
"B               BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
"B                                                                       B",
"B                                                                       B",
"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB                                         B",
"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB   BBBBBBBBBBBBBBBBBBBBBB   BBBBBBBBBBBBBB",
"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB                                         B",
"B                                                                       B",
"B                                                                       B",
"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB"
};

/////// Каждая ячейка это квадрат 32*32
//String TileMap[H] = {
//"BBBBBBBBBBBBBBBB          BBBBBBBBBBBBBB",
//"BBBBBBBBBBBBBBBBBBBBBBB             BBBB",
//"BBBBBBBBBBBBBBBBBBB       BBBBBBBBBBBBBB",
//"BBBBBBBBBBBBBBBB      BBBBBBBBBBBBBBBBBB",
//"BBBBBBBBBBBBBBBB    BBBBBBBBBBBBBBBBBBBB",
//"B                                B     B",
//"B                                B     B",
//"B                                B     B",
//"B                                B     B",
//"B          ZZZ                   B     B",
//"B                                B     B",
//"B                                B     B",
//"B                             BBBB     B",
//"BBBB                             B     B",
//"BBBB                             B     B",
//"B              BB                BB    B",
//"B              BB                      B",
//"B              BB                      B",
//"B    B         BB         BB           B",
//"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
//};

enum class Direction {
        Top,
        Bottom,
        Left,
        Right
};


class PairRange {
    private:
        pair<size_t, size_t> m_start;
        pair<size_t, size_t> m_end;

        int8_t m_order_by_first = true;
        bool m_include_end;

    public:

};

data_types::Range get_collision_blocks_x(const Vector2f& player_pos, const FloatRect& player_bounds) {
    /**
     *    left_x_block:
     *      the block on which the player "left" border is located (by axis X);
     *      - using floor() to round down;
     *    right_x_block:
     *      the block on which the "right" player is located (by axis X);
     *      - using ceil() to round up;
     * */
    int left_x_block = floor(player_pos.x / BLOCK_SIZE);
    int right_x_block = ceil((player_pos.x + player_bounds.width) / BLOCK_SIZE);

    return data_types::Range(left_x_block, right_x_block);
}

data_types::Range get_collision_blocks_y(const Vector2f& player_pos, const FloatRect& player_bounds) {
    /**
     *    top_y_block:
     *      the block on which the player "upper" border is located (by axis Y);
     *    bottom_y_block:
     *      the block on which the "lower" player is located (by axis Y);
     * */
    int top_y_block = floor(player_pos.y / BLOCK_SIZE);
    int bottom_y_block = ceil((player_pos.y + player_bounds.height) / BLOCK_SIZE);

    return data_types::Range(top_y_block, bottom_y_block);
}

pair<int, int> get_adjacent_block(Direction dir, const Vector2f& player_pos, const FloatRect& player_bounds) {
    int start_y = std::floor(player_pos.y / BLOCK_SIZE);
    int end_y = std::floor((player_pos.y + player_bounds.height) / BLOCK_SIZE);
    int center_y = (start_y + end_y) / 2;

    int start_x = std::floor(player_pos.x / BLOCK_SIZE);
    int end_x = std::floor((player_pos.x + player_bounds.width) / BLOCK_SIZE);
    int center_x = (start_x + end_x) / 2;

    switch (dir) {
        case Direction::Top: {
            int top_block = start_y - 1;

            return pair(
               center_x,
               top_block > 0 ? top_block : 0
            );
        }
        case Direction::Right: {
            int right_block = end_x + 1;

            return pair(
               right_block > W ? W : right_block,
               center_y
            );
        }
        case Direction::Bottom: {
            int bottom_block = end_y + 1;

            return pair(
                center_x,
                bottom_block > H ? H : bottom_block
            );
        }
        case Direction::Left: {
            int left_block = start_x - 1;

            return pair(
               left_block > 0 ? left_block : 0,
               center_y
            );
        }
        default:
            throw std::runtime_error("unknown param");
    }
}

map<Direction, pair<int, int>> get_around_blocks(const Vector2f& player_pos, const FloatRect& player_bounds) {
    array<Direction, 4> directions {
      Direction::Top,
      Direction::Right,
      Direction::Bottom,
      Direction::Left
    };

    map<Direction, pair<int, int>> around_blocks {};
    for (auto& direction : directions) {
        around_blocks[direction] = get_adjacent_block(direction, player_pos, player_bounds);
    }

    return around_blocks;
}

float get_hitting_in_texture(
  const Direction direction,
  const Vector2f& player_pos,
  const FloatRect& player_bounds,
  size_t block
) {
    // X - start of contact block
    // Y - start of contact block
    float block_position = block * BLOCK_SIZE;

    switch (direction) {
        case Direction::Right: {
            float x = player_pos.x + player_bounds.width;  // position with width
            float y = block_position;                      // start of contact block by X

            float result = y - x;
            if(result > 0) return 0.f;                     // must be negative because block after player position

            return result;
        }
        case Direction::Left: {
            float x = player_pos.x;                 // player position (start of sprite)
            float y = block_position + BLOCK_SIZE;  // end of contact block by X

            float result = y - x;
            if(result < 0) return 0.f;

            return result;
        }
        case Direction::Top: {
            float x = player_pos.y;                  // player position (start of sprite)
            float y = block_position + BLOCK_SIZE;   // end of contact block by Y

            float result = y - x;
            if(result < 0) return 0.f;

            return result;
        }
        case Direction::Bottom: {
            float x = player_pos.y + player_bounds.height;  // position with height
            float y = block_position;                       // start of contact block by Y

            float result = y - x;
            if(result > 0) return 0.f;                      // must be negative because block after player position

            return result;
        }
    }

    return 0.f;
}

class Player {
    private:
        IntRect dx_rect;
        IntRect dy_rect;

        float dx_current_frame, dy_current_frame;
        float animation_change_rate;

        Texture texture;

    public:
        Vector2f position;

        enum Axis {
            Zero,
            X,
            Y
        };

        float dx, dy; // speed
        bool run;

        Sprite sprite;
        Axis prev_axis = Axis::Zero;

    public:
        Player() {
            texture.loadFromFile("/home/user/Code/stud-game/data/textures/user3.png");

            position = Vector2f(BLOCK_SIZE * 1, BLOCK_SIZE * 1);

            cout <<"position.x = " << position.x << endl;
            cout <<"position.y = " << position.y << endl;

            dx_rect = IntRect(16, 225, 85, 65);
            dy_rect = IntRect(20, 5, 90, 95);

            sprite.setTexture(texture);
            sprite.setTextureRect(dx_rect);
            sprite.setPosition({ position.x - offsetX + PADDINGS.left, position.y - offsetY + PADDINGS.top });
            sprite.scale(0.3, 0.3);

            dx = dy = 0.0;

            dx_current_frame = 1;
            dy_current_frame = 0;

            animation_change_rate = 0.0035;
            run = false;
        };
        ~Player() = default;

        void collision(Axis axis, float prev_pos) {
            show_text = false;

            /**
             *  using "getGlobalBounds" because we use "sprite.scale"
             * */
            const auto& bounds = sprite.getGlobalBounds();

            const auto& collision_blocks_y = get_collision_blocks_y(position, bounds);
            const auto& collision_blocks_x = get_collision_blocks_x(position, bounds);

            for (size_t i = collision_blocks_y.get_start(); i < collision_blocks_y.get_end(); i++) {
                for (size_t j = collision_blocks_x.get_start(); j < collision_blocks_x.get_end(); j++) {
                    char32_t cell = TileMap[i][j];

                    if(cell != 'B' && cell != 'Z') continue;;

                    if(axis == Axis::X) {
                        if(dx > 0) position.x = prev_pos;
                        if(dx < 0) position.x = prev_pos;
                    }

                    if(axis == Axis::Y) {
                        if(dy > 0) position.y = prev_pos;
                        if(dy < 0) position.y = prev_pos;
                    }
                }
            }

            // START
            const auto& around_blocks = get_around_blocks(position, sprite.getGlobalBounds());
            for(auto& around_block: around_blocks) {
                if(show_text) break;

                auto& block = around_block.second;

                auto x = block.first;
                auto y = block.second;

                switch (around_block.first) {
                    case Direction::Top:
                    case Direction::Bottom: {
                        int x_l = x - 1;
                        int x_r = x + 1;

                        char32_t cell_c = TileMap[y][x];
                        char32_t cell_l = TileMap[y][x_l > 0 ? x_l : 0];
                        char32_t cell_r = TileMap[y][x_r > W ? W : x_r];

                        if(cell_c == 'Z' || cell_l == 'Z' || cell_r == 'Z') {
                            show_text = true;
                        }

                        break;
                    }
                    case Direction::Right:
                    case Direction::Left: {
                        int y_t = y - 1;
                        int y_b = y + 1;

                        char32_t cell_c = TileMap[y][x];
                        char32_t cell_t = TileMap[y_t > 0 ? y_t : 0][x];
                        char32_t cell_b = TileMap[y_b > H ? H : y_b][x];

                        if(cell_c == 'Z' || cell_t == 'Z' || cell_b == 'Z') {
                            show_text = true;
                        }

                        break;
                    }
                }
            }
            // END
        }

        void hitting_in_texture() {
            /**
             *    Explanation:
             *      when we move to the left / right (near the borders)
             *      and then we start to move forward,
             *      then we change the height of the player and get into the texture
             *
             *    Need execute if the width / height of the sprite changes
             * */

            const auto& bounds = sprite.getGlobalBounds();

            const auto& collision_blocks_y = get_collision_blocks_y(position, bounds);
            const auto& collision_blocks_x = get_collision_blocks_x(position, bounds);

            for (size_t i = collision_blocks_y.get_start(); i < collision_blocks_y.get_end(); ++i) {
                for (size_t j = collision_blocks_x.get_start(); j < collision_blocks_x.get_end(); ++j) {
                    char32_t cell = TileMap[i][j];
                    if(cell != 'B') continue;

                    float x_block_position = j * BLOCK_SIZE;
                    float y_block_position = i * BLOCK_SIZE;

                    bool is_right = x_block_position >= position.x;
                    bool is_left = !is_right;

                    bool is_top = y_block_position <= position.y;
                    bool is_bottom = !is_top;

                    bool is_X = is_right || is_left;
                    bool is_Y = is_top || is_bottom;

                    float hitting_x;
                    float hitting_y;

                    if(is_X) {
                        if(is_right)
                            hitting_x = get_hitting_in_texture(Direction::Right, position, bounds, j);
                        else
                            hitting_x = get_hitting_in_texture(Direction::Left, position, bounds, j);
                    }

                    if(is_Y) {
                        if(is_top)
                            hitting_y = get_hitting_in_texture(Direction::Top, position, bounds, i);
                        else
                            hitting_y = get_hitting_in_texture(Direction::Bottom, position, bounds, i);
                    }

                    if(is_X && is_Y) {
                        float result;
                        if(hitting_x < 0 && hitting_y < 0) {
                            result = std::max(hitting_x, hitting_y);
                        } else {
                            float fabs_hitting_x = fabs(hitting_x);
                            float min = std::min(fabs_hitting_x, fabs(hitting_y));

                            if(min == fabs_hitting_x)
                                result = hitting_x;
                            else
                                result = hitting_y;
                        }

                        if(result == hitting_x)
                            hitting_y = 0;
                        else
                            hitting_x = 0;
                    }

                    if(hitting_x != 0) {
                        if(hitting_x > 0) position.x += (hitting_x + 5);
                        else position.x += (hitting_x - 5);
                    }

                    if(hitting_y != 0) {
                        if(hitting_y > 0) position.y += (hitting_y + 2);
                        else position.y += (hitting_y - 2);
                    }
                }
            }
        };

        void update(float time) {
            bool have_movement = (dx != 0 || dy != 0);
            if(!have_movement) return;

            Axis axis = dx != 0 ? Axis::X : Axis::Y;

            /** PLAYER MOVEMENT */
            float boost = 0;
            if(run) {
                boost = 0.05 * time; /// a * t; a - ускорение, t - время

                if(axis == Axis::X) {
                    if(dx > 0) dx = dx + boost;
                    if(dx < 0) dx = dx - boost;
                }
                if(axis == Axis::Y) {
                    if(dy > 0) dy = dy + boost;
                    if(dy < 0) dy = dy - boost;
                }
            }

            auto prev_x_pos = position.x;
            auto prev_y_pos = position.y;

//            cout << "dx * time = " << dx * time << endl;
            position.x += dx * time;
            this->collision(Axis::X, prev_x_pos);

//            cout << "dy * time = " << dy * time << endl;
            position.y += dy * time;
            this->collision(Axis::Y, prev_y_pos);

            cout <<"position.x = " << position.x << endl;
            cout <<"position.y = " << position.y << endl;

            /** ANIMATION */
            float S_animation = animation_change_rate * time;
            if(boost != 0) S_animation = S_animation + (0.0015 * time);

            auto& frame = axis == Axis::X
               ? dx_current_frame
               : dy_current_frame;

            frame += S_animation;
            if(frame > 4) frame -= 4;

            if(axis == Axis::X) {
                /**
                 *      обычное отображение - IntRect(10 + ((int)dx_current_frame * 100), 215, 100, 70)
                 *      зеркальное - IntRect(10 + ((int)dx_current_frame * 100) + 100, 215, -100, 70)
                 * */

                if(dx > 0) {
                    dx_rect.left = 18 + ((int)dx_current_frame * 100);
                    dx_rect.width = 85;
                }
                if(dx < 0) {
                    dx_rect.left = 18 + ((int)dx_current_frame * 100) + 100;
                    dx_rect.width = -85;
                }

                sprite.setTextureRect(dx_rect);
                this->hitting_in_texture();
            } else {
                dy_rect.left = 20 + (int(dy_current_frame) * 100);

                if(dy > 0) {
                    dy_rect.top = 5;
                    dy_rect.height = 95;
                }
                if(dy < 0) {
                    dy_rect.top = 5 + 95;
                    dy_rect.height = -95;
                }

                sprite.setTextureRect(dy_rect);
                this->hitting_in_texture();
            }


            sprite.setPosition({ position.x - offsetX + PADDINGS.left, position.y - offsetY + PADDINGS.top });
//            sprite.setPosition({ position.x - offsetX - PADDINGS.left, position.y - offsetY - PADDINGS.top });
//            sprite.setPosition({ position.x - offsetX.get_value(), position.y - offsetY.get_value() });

            prev_axis = axis;
            dx = dy = 0;
        }
};

class Strategy {

};

class StrategiesController {

};

int main() {
    RenderWindow window( VideoMode(WINDOW_W, WINDOW_H), "Test!");
    using namespace form::types;


    form::types::Text text { "Click [x] to open door" };
//    form::types::Button button { { 150, 80 }, "" };
//    form::types::TextBox text_box { {100, 30}, "TEXT" };
//
//    button
//        .set_window_size(window.getSize())
//        .button_text_to_center()
//        .to_center(form::types::Element::XY)
//        .correct_position(true)
//        .border_with_position(true)
//        .set_border_color(sf::Color::Black)
//        .set_border_width(3)
//        .build();

    text
     .set_text_size(22)
     .set_window_size(window.getSize())
     .correct_position(true)
     .to_center(Element::XY)
     .move(Element::Y, WINDOW_H / 3)
     .build();

//    form::types::Element::to_center(button, text);
//
//    text_box
//        .set_max_chars_number(10)
//        .set_window_size(window.getSize())
//        .to_center(form::types::Element::X)
//        .set_after(form::types::Element::Y, button)
//        .move(form::types::Element::Y, 20)
//        .correct_position(true)
//        .border_with_position(true)
//        .set_border_color(sf::Color::Black)
//        .set_border_width(2)
//        .build();


    engine::GameMap mini_map(MINI_BLOCK_SIZE);

    Player player;

    RectangleShape WIN { Vector2f(350, 200) };
    WIN.setFillColor(sf::Color::Blue);
    WIN.setPosition( (WINDOW_W / 2.f) - 350 / 2.f, (WINDOW_H / 2.f) - 100);

    RectangleShape rectangle(Vector2f(BLOCK_SIZE, BLOCK_SIZE));
    RectangleShape mini_rectangle(Vector2f(MINI_BLOCK_SIZE, MINI_BLOCK_SIZE));

    rectangle.setOutlineColor(Color::Black);
    rectangle.setOutlineThickness(1);

    bool show_win = false;

    Clock clock;
    while (window.isOpen()) {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();

        time = time / 600;

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();
//            if (event.key.code == Keyboard::Escape) window.close();

            if(show_text && Keyboard::isKeyPressed(sf::Keyboard::X)) {
                show_win = true;
            } else {
                show_win = false;
            }

            player.run = event.key.shift;
        }


        float speed = 0.1;
        if(Keyboard::isKeyPressed(Keyboard::Left)) {
            player.dx = -speed;
        } else if(Keyboard::isKeyPressed(Keyboard::Right)) {
            player.dx = speed;
        } else if(Keyboard::isKeyPressed(Keyboard::Up)) {
            player.dy = -speed;
        } else if(Keyboard::isKeyPressed(Keyboard::Down)) {
            player.dy = speed;
        }

        player.update(time);

//        auto res = get_adjacent_block(Direction::Top, player.position, player.sprite.getGlobalBounds());
//        auto res2 = get_adjacent_block(Direction::Left, player.position, player.sprite.getGlobalBounds());
//        auto res3 = get_adjacent_block(Direction::Right, player.position, player.sprite.getGlobalBounds());
//        auto res4 = get_adjacent_block(Direction::Bottom, player.position, player.sprite.getGlobalBounds());
//        cout << "top = " << res.second << endl;
//        cout << "left = " << res2.first << endl;
//        cout << "right = " << res3.first << endl;
//        cout << "bottom = " << res4.second << endl;

        if(player.position.x >= (W * BLOCK_SIZE) - WINDOW_W / 2.f) {
            // nothing
        } else if(player.position.x > WINDOW_W / 2.f) {
//            offsetX.set_value(player.position.x - WINDOW_W / 2.f);
            offsetX = player.position.x - WINDOW_W / 2.f;
        }

//        if(player.position.y >= (H * BLOCK_SIZE) - WINDOW_H / 2.f) {
//            // nothing
//        } else if(player.position.y > WINDOW_H / 2.f) {
////            offsetY.set_value(player.position.y - WINDOW_H / 2.f);
//            offsetY = player.position.y - WINDOW_H / 2.f;
//        }

        window.clear(Color::Yellow);

        for(int i = 0; i < H; i++) {
            for(int j = 0; j < W; j++) {
                // MAIN MAP
                if(TileMap[i][j] == ' ') rectangle.setFillColor(Color::White);
                if(TileMap[i][j] == 'B') {
                    rectangle.setFillColor(Color::Black);
                    rectangle.setOutlineColor(Color::White);
                }
                if(TileMap[i][j] == 'Z') {
                    rectangle.setFillColor(Color::Yellow);
                }

//                rectangle.setPosition((j * BLOCK_SIZE) - offsetX.get_value(), (i * BLOCK_SIZE) + offsetY.get_value());
                rectangle.setPosition((j * BLOCK_SIZE) - offsetX + PADDINGS.left, (i * BLOCK_SIZE) + offsetY + PADDINGS.top);
//                rectangle.setPosition((j * BLOCK_SIZE) - offsetX, (i * BLOCK_SIZE) - offsetY);
                window.draw(rectangle);

                rectangle.setOutlineColor(Color::Black);
            }
        }


        auto player_bounds = player.sprite.getGlobalBounds();
        int player_i = int((player.position.y + (player_bounds.height / 2.f)) / BLOCK_SIZE);
        int player_j = int((player.position.x + (player_bounds.width / 2.f)) / BLOCK_SIZE);


        int start_x, end_x;
        start_x = std::max(0, player_j - MAX_BLOCK_VISING_X / 2);
        end_x = std::min(W, player_j + MAX_BLOCK_VISING_X / 2);

        for(int i = 0; i < H; i++) {
            int offset = 5;
            float offsetX = start_x * MINI_BLOCK_SIZE;


            for(int j = start_x; j < end_x; j++) {
                // MINI MAP
                if(TileMap[i][j] == 'B') {
                    mini_rectangle.setFillColor(Color::Green);
                }
                if(TileMap[i][j] == 'Z') {
                    mini_rectangle.setFillColor(Color::Yellow);
                }
                if(player_i == i && player_j == j) {
                    mini_rectangle.setFillColor(Color::Red);
                }

                mini_rectangle.setPosition((j * MINI_BLOCK_SIZE) + offset - offsetX, (i * MINI_BLOCK_SIZE) + offset);
                window.draw(mini_rectangle);

                mini_rectangle.setFillColor(Color::White);
            }
        }

        if(show_text) {
            text.draw(window);
        }
//        button.draw(window);
//        text_box.draw(window);

        window.draw(player.sprite);
        if(show_win) {
            window.draw(WIN);
        }

        window.display();
    }

    return EXIT_SUCCESS;
}

//        if(Keyboard::isKeyPressed(Keyboard::Left)) {
//            float speed = -0.1;
//            sprite.move(speed * time, 0);
//
//            dx_current_frame += S_animation;
//            if(dx_current_frame > 4) dx_current_frame -= 4;
//
//            /**
//             *      обычное отображение - IntRect(10 + ((int)dx_current_frame * 100), 215, 100, 70)
//             *      зеркальное - IntRect(10 + ((int)dx_current_frame * 100) + 100, 215, -100, 70)
//             * */
//            sprite.setTextureRect(IntRect(10 + ((int)dx_current_frame * 100) + 100, 215, -100, 70));
//        } else if(Keyboard::isKeyPressed(Keyboard::Right)) {
//            float speed = 0.1;
//            sprite.move(speed * time, 0);
//
//            dx_current_frame += S_animation;
//            if(dx_current_frame > 4) dx_current_frame -= 4;
//
//            sprite.setTextureRect(IntRect(10 + ((int)dx_current_frame * 100), 215, 100, 70));
//        } else if(Keyboard::isKeyPressed(Keyboard::Up)) {
//            float speed = -0.1;
//            sprite.move(0, speed * time);
//
//            dy_current_frame += S_animation;
//            if(dy_current_frame > 4) dy_current_frame -= 4;
//
//            sprite.setTextureRect(IntRect(20 + (int(dy_current_frame) * 100), 5 + 95, 90, -95));
//        } else if(Keyboard::isKeyPressed(Keyboard::Down)) {
//            float speed = 0.1;
//            sprite.move(0, speed * time);
//
//            dy_current_frame += S_animation;
//            if(dy_current_frame > 4) dy_current_frame -= 4;
//
//            sprite.setTextureRect(IntRect(20 + (int(dy_current_frame) * 100), 5, 90, 95));
//        }

int main_forms() {
    // Объект, который, собственно, является главным окном приложения
    RenderWindow window(VideoMode(400, 400), "SFML Works!");

    using namespace form::types;

    form::types::TextBox input {{100, 30 }, "text" };
    input
        .set_max_chars_number(10)
        .set_window_size(window.getSize())
        .to_center(form::types::TextBox::XY)
        .correct_position(true)
        .border_with_position(true)
        .set_border_color(sf::Color::Black)
        .set_border_width(2);

    form::types::Button button {{100, 20 }, "*" };
    button
            .set_window_size(window.getSize())
            .to_center(form::types::TextBox::XY)
            .correct_position(true)
            .border_with_position(true)
            .set_border_color(sf::Color::Black)
            .set_border_width(2)
            .set_after(Element::XY, input)
            .build();

    button.button_text_to_center();

    input.build();

    auto& call_scheduler_controller = engine::controllers::CallSchedulerController::get_instance();
    auto& plan = call_scheduler_controller.schedule(600,  [&]() {
        if(!input.is_focused()) return;

        auto& text = input.get_button_text();
        const auto& size = text.get_size();

        auto last_pos = size > 0 ? size - 1 : 0;

        if(input[last_pos] == '|') {
            text.narrow_text(0, 1);
        } else {
            text.add_char('|');
        }

        input.build();
    });

//    input.on_click(
//      [](form::types::Element& el) {
//          auto& input = dynamic_cast<form::types::TextBox&>(el);
//
//          input.set_border_color(sf::Color::Yellow);
//          input.build();
//      },
//      [](form::types::Element& el) {
//          auto& input = dynamic_cast<form::types::TextBox&>(el);
//
//          input.set_border_color(sf::Color::Black);
//          input.build();
//      }
//    );

    input.on_focus(
      [&plan](form::types::Element& el) {
          el
              .set_border_color(sf::Color::Red)
              .build();

          plan.activate();
      },
      [&plan](form::types::Element& el) {
          auto& input = dynamic_cast<TextBox&>(el);

          auto& text = input.get_button_text();
          const auto& size = text.get_size();

          auto last_pos = size > 0 ? size - 1 : 0;

          if(input[last_pos] == '|') {
              text.narrow_text(0, 1);
          }

          el
            .set_border_color(sf::Color::Black)
            .build();

          plan.disable();
      }
    );

    auto& focus_controller = engine::controllers::FocusController::get_instance();
    focus_controller
        .set_window(&window)
        .register_element(&input);

    auto& mouse_position = engine::data_utils::MousePosition::get_instance(&window);

    try {
        // Главный цикл приложения: выполняется, пока открыто окно
        while (window.isOpen())
        {
            // Обрабатываем события в цикле
            Event event;
            while (window.pollEvent(event))
            {
                focus_controller.input(event);
                engine::data_utils::MousePosition::input(mouse_position, window, event);

                form::types::TextBox::input(input, window, event);
                form::types::Button::input(input, window, event, mouse_position.get_prev_pos());

                // Пользователь нажал на «крестик» и хочет закрыть окно?
                if (event.type == Event::Closed)
                    // тогда закрываем его
                    window.close();
            }

            call_scheduler_controller.call();

            // Установка цвета фона - белый
            window.clear(Color::White);

            input.draw(window);
            button.draw(window);

            // Отрисовка окна
            window.display();
        }
    } catch (std::exception &ex) {
        cerr << ex.what();
    }


    return 0;
}

int main_original()
{


//        try {
//            FileWriter f_w("./data/json/level_1.json");
//
//            f_w.open_file();
//            f_w.write("test");
//            f_w.close_file();
//
//            //cout << "name => " << f_r.to_json()["name"] << endl;
//        } catch (Exception& ex) {
//            ex.draw_error();
//        } catch (std::exception &ex) {
//            cerr << ex.what();
//        }

//    auto *button = new InterfaceForm::Button{ 50, 50, "text" };
//    // reinterpret_cast<void*>(boo)
//    button->onClick = []() {
//       cout << "Onclick" << endl;
//    };
//    button->onClick();
//    delete button;

//      TODO _exception strategy
//    try {
//       throw Exception(Exception::FileRead, "Testing standart error");
//      // ...
//    } catch (Exception& ex) {
//            ex.draw_error();
//    } catch (std::exception &exception) {
//        std::cerr << "Standard exception: " << exception.what() << '\n';
//    } catch (...) {
//          return EXIT_FAILURE;
//    }


//    int array[4] = { 5, 8, 6, 4 };
//    cout << sizeof(array) << endl;
//
//    int value = 5;
//    int *ptr = new int(value);
//
//    const int *const ptr2 = &value;
//
//    test(&value);
//
//    void *ptr3;
//    ptr3 = &value;
//
//    cout << " b => " << *static_cast<int*>(ptr3);

    // assert(1 == 0);

    //Employee &l_value (Employee);

//    File f("../../././data/json/level_2.json");
//    cout << f.get_file_path() << endl;

//    cout << f.check_file_exists();

//    try {
//        f.read();
//
//        json j2;
//        f.iterate([&j2](char* chunk)  {
//            j2 << *chunk;
//            return 0;
//        });
//
//        json j = json::parse(f.get_data());
//
//
//        cout << j["name"] << endl;
//    } catch (Exception &ex) {
//        ex.out_to(cout);
//    }


//    Game g;
//
//    g.start();

//    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
//    sf::CircleShape shape(100.f);
//    shape.setFillColor(sf::Color::Green);
//
//    while (window.isOpen())
//    {
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            if (event.type == sf::Event::Closed)
//                window.close();
//        }
//
//        window.clear();
//        window.draw(shape);
//        window.display();
//    }

    return EXIT_SUCCESS;
}