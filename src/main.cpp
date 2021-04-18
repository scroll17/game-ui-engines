#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#include "iostream"
#include <map>
#include <cmath>
#include "pugixml.hpp"

#include "./DataTypes/Range/Range.h"

//#include "./Engine/Form/Types/Button/Button.h"
//#include "./Engine/Form/Types/TextBox/TextBox.h"
//
//#include "./Engine/Controllers/FocusController/FocusController.h"
//#include "./Engine/Controllers/CallSchedulerController/CallSchedulerController.h"
//
//#include "./Engine/DataUtils/MousePosition/MousePosition.h"
//
//#include "utils/array/array.h"


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

using json = nlohmann::json;

using namespace std;
using namespace sf;

const int WINDOW_W = 600, WINDOW_H = 400;

float offsetX = 0;
float offsetY = 0;

const int H = 12;  /// Высота
const int W = 40;  /// Длина
const int BLOCK_SIZE = 42; /// Размер блока;

/// Каждая ячейка это квадрат 32*32
String TileMap[H] = {
"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
"B                                B     B",
"B                                B     B",
"B                                B     B",
"B                                B     B",
"B         0000                BBBB     B",
"BBBB                             B     B",
"BBBB                             B     B",
"B              BB                BB    B",
"B              BB                      B",
"B    B         BB         BB           B",
"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
};


// TODO _2 create functions for get blocks with collision by (X, Y) ex: get_blocks_with_collision(bounds)

Range get_collision_blocks_x(const Vector2f& player_pos, const FloatRect& player_bounds) {
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

    return Range(left_x_block, right_x_block);
}

Range get_collision_blocks_y(const Vector2f& player_pos, const FloatRect& player_bounds) {
    /**
     *    top_y_block:
     *      the block on which the player "upper" border is located (by axis Y);
     *    bottom_y_block:
     *      the block on which the "lower" player is located (by axis Y);
     * */
    int top_y_block = floor(player_pos.y / BLOCK_SIZE);
    int bottom_y_block = ceil((player_pos.y + player_bounds.height) / BLOCK_SIZE);

    return Range(top_y_block, bottom_y_block);
}

enum class Direction {
    Top,
    Bottom,
    Left,
    Right
};

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

//    switch (direction) {
//        case Direction::Right: {
//            float x = player_pos.x + player_bounds.width;  // position with width
//            float y = x - block_position;                  // hitting the texture
//
//            float result = player_pos.x - (player_pos.x - y);
//            cout << "right 2 = " << -result << endl;
//
//            if(result < 0) return 0;
//
//            return -result;
//        }
//        case Direction::Left: {
//            float x = player_pos.x;                 // player position (start of sprite)
//            float y = block_position + BLOCK_SIZE;  // end of contact block
//
//            float result = y - x;
//            if(result < 0) return 0;
//
//            return result;
//        }
//        case Direction::Top: {
//            break;
//        }
//        case Direction::Bottom: {
//            break;
//        }
//    }
}

//class GameMap {
//    private:
//        string *m_tile { nullptr };
//
//    public:
//        GameMap();
//        ~GameMap();
//
//        bool load_tile();
//
//        Range get_collision_blocks_x(const Vector2f& player_pos, const FloatRect& player_bounds);
//        Range get_collision_blocks_y(const Vector2f& player_pos, const FloatRect& player_bounds);
//};

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

            position = Vector2f(WINDOW_W / 2.f, WINDOW_H / 2.f);

            dx_rect = IntRect(16, 225, 85, 65);
            dy_rect = IntRect(20, 5, 90, 95);

            sprite.setTextureRect(dx_rect);
            sprite.setPosition(position);
            sprite.setTexture(texture);
            sprite.scale(0.8, 0.8);

            dx = dy = 0.0;

            dx_current_frame = 1;
            dy_current_frame = 0;

            animation_change_rate = 0.0035;
            run = false;
        };
        ~Player() = default;

        // TODO _
//        void set_direction(const Axis& axis);
//        void get_direction();
//        bool direction_has_be_changed();

        void collision_x(float prev_x_pos) {
            /**
             *  using "getGlobalBounds" because we use "sprite.scale"
             * */
            const auto& bounds = sprite.getGlobalBounds();

            const auto& collision_blocks_y = get_collision_blocks_y(position, bounds);
            const auto& collision_blocks_x = get_collision_blocks_x(position, bounds);

            for (size_t i = collision_blocks_y.get_start(); i < collision_blocks_y.get_end(); i++) {
                for (size_t j = collision_blocks_x.get_start(); j < collision_blocks_x.get_end(); j++) {
                    char32_t cell = TileMap[i][j];

                    if(cell != 'B') continue;;

                    if(dx > 0) position.x = prev_x_pos;
                    if(dx < 0) position.x = prev_x_pos;
                }
            }
        }

        void collision_y(float prev_y_pos) {
            /**
             *  using "getGlobalBounds" because we use "sprite.scale"
             * */
            const auto& bounds = sprite.getGlobalBounds();

            const auto& collision_blocks_y = get_collision_blocks_y(position, bounds);
            const auto& collision_blocks_x = get_collision_blocks_x(position, bounds);

            for (size_t i = collision_blocks_y.get_start(); i < collision_blocks_y.get_end(); i++) {
                for (size_t j = collision_blocks_x.get_start(); j < collision_blocks_x.get_end(); j++) {
                    char32_t cell = TileMap[i][j];

                    if(cell != 'B') continue;;

                    if(dy > 0) position.y = prev_y_pos;
                    if(dy < 0) position.y = prev_y_pos;
                }
            }
        }

        /**
         *      TODO _need realesed
         *          когда мы перемещаемся в лево/право (в близи границ)
         *          а после чего начинаем двигаться в перёд то мы изменяем высоту игрока и попадаем в текстуру
         *
         *          Как исправить:
         *              - следить за предыдущим направлением движения и проверять если мы изменили направление
         *              - если мы попали в текстуру то нас нужно откинуть ровно на столько на сколько мы находимся в текстуре
         * */
        void hitting_in_texture(Axis axis) {
            // TODO - execute if the width / height of the sprite changes

            const auto& bounds = sprite.getGlobalBounds();

            const auto& collision_blocks_y = get_collision_blocks_y(position, bounds);
            const auto& collision_blocks_x = get_collision_blocks_x(position, bounds);

            cout << "----------------- hitting_in_texture ---------------------" << endl;

            for (size_t i = collision_blocks_y.get_start(); i < collision_blocks_y.get_end(); ++i) {
                for (size_t j = collision_blocks_x.get_start(); j < collision_blocks_x.get_end(); ++j) {
                    char32_t cell = TileMap[i][j];
//                    char cell2 = cell;

//                    cout << "TileMap["
//                         << i << ", " << j
//                         << "] = " << cell2
//                         << endl;

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

                    cout << "1) is_X = " << is_X << endl;
                    cout << "2) is_Y = " << is_Y << endl;

                    if(is_X) {
                        cout << "3) is_right = " << is_right << endl;
                        cout << "4) is_left = " << is_left << endl;

                        if(is_right)
                            hitting_x = get_hitting_in_texture(Direction::Right, position, bounds, j);
                        else
                            hitting_x = get_hitting_in_texture(Direction::Left, position, bounds, j);

                        cout << "5) hitting_x = " << hitting_x << endl;
                    }

                    if(is_Y) {
                        cout << "6) is_top = " << is_top << endl;
                        cout << "7) is_bottom = " << is_bottom << endl;

                        if(is_top)
                            hitting_y = get_hitting_in_texture(Direction::Top, position, bounds, i);
                        else
                            hitting_y = get_hitting_in_texture(Direction::Bottom, position, bounds, i);

                        cout << "8) hitting_y = " << hitting_y << endl;
                    }

                    if(is_X && is_Y) {
                        float result;
                        if(hitting_x < 0 && hitting_y < 0) {
                            result = std::max(hitting_x, hitting_y);
                        } else {
                            float min = std::min(fabs(hitting_x), fabs(hitting_y));

                            if(min == fabs(hitting_x))
                                result = hitting_x;
                            else
                                result = hitting_y;
                        }


                        cout << "9) result = " << result << endl;

                        if(result == hitting_x)
                            hitting_y = 0;
                        else
                            hitting_x = 0;
                    }

                    if(hitting_x != 0) {
                        cout << "10) hitting_x != 0 = " << (hitting_x != 0) << endl;
                        if(hitting_x > 0) {
                            cout << "11) (hitting_x + 5) = " << (hitting_x + 5) << endl;
                            position.x += (hitting_x + 5);
                        }
                        else {
                            cout << "12) (hitting_x - 5) = " << (hitting_x - 5) << endl;
                            position.x += (hitting_x - 5);
                        }

                        cout << "13) position.x = " << position.x << endl;
                    }

                    if(hitting_y != 0) {
                        if(hitting_y > 0) {
                            cout << "14) (hitting_y + 2) = " << (hitting_y + 2) << endl;
                            position.y += (hitting_y + 2);
                        }
                        else {
                            cout << "15) (hitting_y - 2 = " << (hitting_y - 2) << endl;
                            position.y += (hitting_y - 2);
                        }

                        cout << "16) position.y = " << position.y << endl;
                    }


////                    if(axis == Axis::X) {
//                        cout << "x_block_position >= position.x === " << (x_block_position >= position.x) << endl;
//                        cout << "x_block_position <= position.x === " << (x_block_position <= position.x) << endl;
//
//                        if(x_block_position >= position.x) {
//                            float hitting = get_hitting_in_texture(Direction::Right, position, bounds, j);
//                            if(hitting != 0) position.x += (hitting - 5);
//
//                            cout << "Direction::Right => " << hitting << endl;
//
//                        } else if(x_block_position <= position.x) {
//                            float hitting = get_hitting_in_texture(Direction::Left, position, bounds, j);
//                            if(hitting != 0) position.x += (hitting + 5);
//
//                            cout << "Direction::Left => " << hitting << endl;
//                        }
////                    }
//
////                    if(axis == Axis::Y) {
//                        cout << " y_block_position <= position.y === " << (y_block_position <= position.y) << endl;
//                        cout << " y_block_position >= position.y === " << (y_block_position >= position.y) << endl;
//
//                        if(y_block_position >= position.y) {
//                            float hitting = get_hitting_in_texture(Direction::Bottom, position, bounds, i);
//                            if(hitting != 0) position.y += (hitting - 2);
//
//                            cout << "Direction::Bottom => " << hitting << endl;
//                        } else if(y_block_position <= position.y) {
//                            float hitting = get_hitting_in_texture(Direction::Top, position, bounds, i);
//                            if(hitting != 0) position.y += (hitting + 2);
//
//                            cout << "Direction::Top => " << hitting << endl;
//                        }
////                    }
                }
            }

            cout << "----------------- hitting_in_texture ---------------------" << endl;
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

            position.x += dx * time;
//            cout << "position.x = " << position.x << endl;
            this->collision_x(prev_x_pos);

            const auto& bounds = sprite.getGlobalBounds();

            position.y += dy * time;
//            cout << "position.y = " << position.y << endl;
//            cout << "bounds.top = " << bounds.top << endl;
            this->collision_y(prev_y_pos);

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
            }


            if(prev_axis != axis && prev_axis != Axis::Zero) {
//                if(prev_axis == Axis::X) this->hitting_in_texture(Axis::Y);
//                if(prev_axis == Axis::Y) this->hitting_in_texture(Axis::X);
//                this->hitting_in_texture(Axis::X);
                this->hitting_in_texture(Axis::Y);
            }

            sprite.setPosition({ position.x - offsetX, position.y - offsetY });

            prev_axis = axis;
            dx = dy = 0;
        }
};

int main() {
    RenderWindow window( VideoMode(WINDOW_W, WINDOW_H), "Test!");

    Player player;
    Clock clock;

    RectangleShape rectangle(Vector2f(BLOCK_SIZE, BLOCK_SIZE));
    rectangle.setOutlineColor(Color::Black);
    rectangle.setOutlineThickness(1);

    while (window.isOpen()) {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();

        time = time / 600;


        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();
            if (event.key.code == Keyboard::Escape) window.close();

            player.run = event.key.shift;
        }


        if(Keyboard::isKeyPressed(Keyboard::Left)) {
            player.dx = -0.2;
        } else if(Keyboard::isKeyPressed(Keyboard::Right)) {
            player.dx = 0.2;
        } else if(Keyboard::isKeyPressed(Keyboard::Up)) {
            player.dy = -0.2;
        } else if(Keyboard::isKeyPressed(Keyboard::Down)) {
            player.dy = 0.2;
        }

        player.update(time);

        if(player.position.x > WINDOW_W / 2.f) {
            offsetX = player.position.x - WINDOW_W / 2.f;
        }
        offsetY = player.position.y - WINDOW_H / 2.f;

        window.clear(Color::White);

        for(int i = 0; i < H; i++) {
            for(int j = 0; j < W; j++) {
                if(TileMap[i][j] == ' ') rectangle.setFillColor(Color::White);
                if(TileMap[i][j] == 'B') {
                    rectangle.setFillColor(Color::Black);
                    rectangle.setOutlineColor(Color::White);
                }
                if(TileMap[i][j] == '0') rectangle.setFillColor(Color::Green);

                rectangle.setPosition((j * BLOCK_SIZE) - offsetX, (i * BLOCK_SIZE) - offsetY);
                window.draw(rectangle);

                rectangle.setOutlineColor(Color::Black);
            }
        }

        window.draw(player.sprite);

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

int main_form() {
    // Объект, который, собственно, является главным окном приложения
    RenderWindow window(VideoMode(400, 400), "SFML Works!");

//    TextBox input {{100, 30 }, {100, 100 }, "" };
//    input
//        .set_max_chars_number(10)
//        .set_window_size(window.getSize())
//        .to_center(TextBox::XY)
//        .correct_position(true)
//        .border_with_position(false)
//        .set_border_color(sf::Color::Black)
//        .set_border_width(2)
//        .build();
//
//    auto& call_scheduler_controller = CallSchedulerController::get_instance();
//    auto& plan = call_scheduler_controller.schedule(600,  [&]() {
//        if(!input.is_focused()) return;
//
//        const auto& size = input.get_button_text().get_size();
//        auto last_pos = size > 0 ? size - 1 : 0;
//
//        if(input[last_pos] == '|') {
//            input.narrow_text(0, 1).build();
//        } else {
//            input.get_button_text().add_char('|').build();
//        }
//    });
//
//    input.on_focus(
//      [&](Element& el) {
//          auto& input = dynamic_cast<TextBox&>(el);
//
//          auto& bounds = input.get_bounds();
//
//          auto height_in_one_percent = bounds.height / 100;
//          auto width_in_one_percent = bounds.width / 100;
//
//          input
//              .get_button_text()
//              .set_size({ bounds.width - (width_in_one_percent * 10), bounds.height - (height_in_one_percent * 10) })
//              .build();
//
//          plan.activate();
//      },
//      [&](Element& el) {
//          plan.disable();
//      }
//    );
//
//    auto& focus_controller = FocusController::get_instance();
//    focus_controller
//        .set_window(&window)
//        .register_element(&input);


    // Создаем переменную текстуры
    Texture texture1;
    texture1.loadFromFile("/home/user/Code/stud-game/data/textures/user.png");

    Texture texture2;
    texture2.loadFromFile("/home/user/Code/stud-game/data/textures/user.png");
    texture2.setSmooth(true);


    // Создаем спрайт и устанавливаем ему нашу текстуру
    Sprite sprite1(texture1, IntRect(112, 34, 96, 96));
    Sprite sprite2(texture2, IntRect(112, 34, 96, 96));

    sprite2.move(100, 100);
    sprite2.setColor(Color::Red);
    sprite2.scale(1.5f, 1.9f);


    try {
        // Главный цикл приложения: выполняется, пока открыто окно
        while (window.isOpen())
        {
            // Обрабатываем события в цикле
            Event event;
            while (window.pollEvent(event))
            {
//                focus_controller.input(event);
//                TextBox::input(input, window, event);

                // Пользователь нажал на «крестик» и хочет закрыть окно?
                if (event.type == Event::Closed)
                    // тогда закрываем его
                    window.close();
            }

//            call_scheduler_controller.call();

            // Установка цвета фона - белый
            window.clear(Color::White);

            // Отрисовка спрайта
            window.draw(sprite1);
            window.draw(sprite2);

//            input.draw(window);

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