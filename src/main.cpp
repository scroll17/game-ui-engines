//
// Created by user on 28.04.2021.
//

#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#include <array>
#include <vector>
#include <cmath>
#include "pugixml.hpp"
#include <ctime>
#include <iomanip> // put_time
#include <sstream>  // ostringstream

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
            m_position = Vector2f(m_game_map.get_block_size() * 2, m_game_map.get_block_size() * 2);

            m_rect_x = IntRect(11, 212, 95, 75);
            m_rect_y = IntRect(20, 5, 90, 95);

            const auto& paddings = m_game_map.get_paddings();

            m_texture.loadFromFile("/home/user/Code/stud-game/data/textures/user.png");

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
    const int WINDOW_W = 600;
    const int WINDOW_H = 400;

    const string DIR_PATH = Constants::directory_path;
    const map<int, string> level_paths {
       { 1, File::resolve_path(DIR_PATH, "./data/json/level_1.json") },
       { 2, File::resolve_path(DIR_PATH, "./data/json/level_2.json") },
    };

    try {
        RenderWindow window( VideoMode(WINDOW_W, WINDOW_H), "Игра");

        StrategiesController strategies;
        data_utils::MousePosition& mouse_position = data_utils::MousePosition::get_instance(&window);

        controllers::CallSchedulerController& call_scheduler_controller = controllers::CallSchedulerController::get_instance();
        controllers::FocusController& focus_controller = controllers::FocusController::get_instance();
        focus_controller.set_window(&window);

        /// GLOBAL VAR
        int g_selected_level = -1;
        string g_username {};
        pugi::xml_node current_user {};

        File users_xml("./data/xml/users.xml");
        pugi::xml_document xml_doc = FileReader::read_xml(users_xml);

//        int g_selected_level = 1;
//        string g_username { "user2" };
//        pugi::xml_node current_user = xml_doc.child("users").find_child([&g_username](pugi::xml_node& node) -> bool {
//            return node.child_value("name") == g_username;
//        });

        /// STRATEGIES

        // 1. loading stage
        strategies.register_strategy([&]() -> bool {
            bool button_active = false;
            bool go_next = false;

            Texture t_nuzp;
            t_nuzp.loadFromFile(File::resolve_path(DIR_PATH, "./data/textures/nuzp.png"));

            Sprite s_nuzp;
            s_nuzp.setTexture(t_nuzp);
            s_nuzp.setPosition(
              (WINDOW_W / 2.f) - (s_nuzp.getGlobalBounds().width / 2.f),
              20
            );

            const auto& sprite_bounds = s_nuzp.getGlobalBounds();
            float sprite_center_x = sprite_bounds.left + sprite_bounds.width / 2.f;

            form::types::Text load_text { "ЗАГРУЗКА" };
            load_text
              .set_text_size(22)
              .set_color(sf::Color::White)
              .set_window_size(window.getSize())
              .correct_position(true)
              .to_center(Element::XY)
              .move(Element::Y, 60)
              .build();

            float progress_bar_steps = 200;
            ProgressBar progress_bar({ 150, 5 }, progress_bar_steps);
            progress_bar
              .set_color(sf::Color(148,0,211))
              .set_window_size(window.getSize())
              .correct_position(true)
              .set_after(Element::Y, load_text)
              .move(Element::Y, 20)
              .to_center(Element::X)
              .move(Element::X, -(140 / 2))
              .build();

            Button button {{100, 20 }, "Играть" };
            button
             .set_bg_color(sf::Color(192,192,192))
             .set_window_size(window.getSize())
             .correct_position(true)
             .border_with_position(true)
             .set_border_color(sf::Color::Blue)
             .set_border_width(2)
             .to_center(Element::XY)
             .set_after(Element::Y, progress_bar)
             .move(Element::Y, 30)
             .build();

            button.button_text_to_center();

            progress_bar.after_ready([&button_active, &button]() {
                button
                  .set_bg_color(sf::Color::White)
                  .build();

                button_active = true;
            });

            button.on_click(
              [](Element& el) {},
              [&button_active, &go_next](Element& el) {
                  if(button_active) {
                      go_next = true;
                  }
              }
            );

            auto& plan = call_scheduler_controller.schedule((1000 * 2) / progress_bar_steps,  [&progress_bar]() {
                progress_bar.next();
            });
            plan.activate();

            while (window.isOpen()) {
                /// EVENTS
                Event event {};
                while (window.pollEvent(event)) {
                    if (event.type == Event::Closed) {
                        window.close();
                        return false;
                    }

                    data_utils::MousePosition::input(mouse_position, window, event);
                    Button::input(button, window, event, mouse_position.get_prev_pos());
                }

                call_scheduler_controller.call();

                /// DRAW
                window.clear(Color::Black);

                window.draw(s_nuzp);

                load_text.draw(window);
                progress_bar.draw(window);
                button.draw(window);

                window.display();

                if(go_next) break;
            }

            call_scheduler_controller.remove_scheduled_plan(plan.get_id());

            return true;
        });

        // 2. username input
        strategies.register_strategy([&]() -> bool {
            bool button_active = false;
            bool go_next = false;

            form::types::Text main_text { "Введите ваше имя: " };
            main_text
              .set_text_size(22)
              .set_color(sf::Color::White)
              .set_window_size(window.getSize())
              .correct_position(true)
              .to_center(Element::XY)
              .build();

            TextBox input {{100, 30 }, "user1" };
              input
               .set_max_chars_number(10)
               .set_window_size(window.getSize())
               .to_center(Element::X)
               .set_after(Element::Y, main_text)
               .move(Element::Y, 20)
               .correct_position(true)
               .border_with_position(true)
               .set_border_color(sf::Color::White)
               .set_border_width(2)
               .build();

            Button button {{100, 20 }, "Дальше" };
            button
               .set_bg_color(sf::Color(192,192,192))
               .set_window_size(window.getSize())
               .correct_position(true)
               .border_with_position(true)
               .set_border_color(sf::Color::Blue)
               .set_border_width(2)
               .to_center(Element::XY)
               .set_after(Element::Y, input)
               .move(Element::Y, 30)
               .build();

            button.button_text_to_center();

            button.on_click(
             [](Element& el) {},
             [&button_active, &go_next](Element& el) {
                if(button_active) {
                   go_next = true;
                }
             }
            );

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

            input.on_focus(
              [&plan](form::types::Element& el) {
                   el
                     .set_border_color(sf::Color::Red)
                     .build();

                   plan.activate();
              },
              [&plan](form::types::Element& el) {
                  auto &input = dynamic_cast<TextBox &>(el);

                  auto &text = input.get_button_text();
                  const auto &size = text.get_size();

                  auto last_pos = size > 0 ? size - 1 : 0;

                  if (input[last_pos] == '|') {
                      text.narrow_text(0, 1);
                  }

                  el
                    .set_border_color(sf::Color::White)
                    .build();

                  plan.disable();
              }
            );

            focus_controller.register_element(&input);

            while (window.isOpen()) {
                /// EVENTS
                Event event {};
                while (window.pollEvent(event)) {
                    if (event.type == Event::Closed) {
                        window.close();
                        return false;
                    }

                    if(event.key.code == 13) {
                        string username = input.get_entered_text();

                        const auto &size = username.size();
                        auto last_pos = size > 0 ? size - 1 : 0;

                        if (username[last_pos] == '|') {
                            username.erase(last_pos, 1);
                        }

                        if(username.size() >= 3) {
                            g_username = username;

                            button
                              .set_bg_color(sf::Color::White)
                              .build();

                            button_active = true;
                        }
                    }

                    focus_controller.input(event);
                    data_utils::MousePosition::input(mouse_position, window, event);

                    Button::input(input, window, event, mouse_position.get_prev_pos());
                    TextBox::input(input, window, event);

                    Button::input(button, window, event, mouse_position.get_prev_pos());
                }

                call_scheduler_controller.call();

                /// DRAW
                window.clear(Color::Black);

                main_text.draw(window);
                input.draw(window);
                button.draw(window);

                window.display();

                if(go_next) {
                    int last_id = xml_doc.child("users").attribute("lastId").as_int();

                    auto user = xml_doc.child("users").find_child([&g_username](pugi::xml_node& node) -> bool {
                        return node.child_value("name") == g_username;
                    });
                    if(user.empty()) {
                        xml_doc.child("users").attribute("lastId") = ++last_id;

                        current_user = xml_doc.child("users").append_child("user");
                        current_user.append_attribute("id") = last_id;
                        current_user.append_child("name").append_child(pugi::node_pcdata).set_value(g_username.c_str());
                        current_user.append_child("scores");
                    } else {
                        current_user = user;
                    }

                    break;
                }
            }

            call_scheduler_controller.remove_scheduled_plan(plan.get_id());
            focus_controller.remove_registration(&input);

            return true;
        });

        // 3. main
        strategies.register_strategy([&]() -> bool {
            bool go_start = false;
            bool go_scores = false;
            bool go_exit = false;
            bool go_help = false;

            form::types::Text username_text { "Имя игрока: " };
            username_text
              .set_text_size(16)
              .set_color(sf::Color::Black)
              .set_window_size(window.getSize())
              .correct_position(true)
              .move(Element::Y, 10)
              .to(Element::Angle::Right)
              .move(Element::X, - 60)
              .build();

            form::types::Text username { username_text };
            username
              .set_text(g_username)
              .set_color(sf::Color::Red)
              .set_after(Element::X, username_text)
              .build();

            Button start_button {{100, 40 }, "Старт" };
            start_button
              .set_text_size(20)
              .set_bg_color(sf::Color(192,192,192))
              .set_window_size(window.getSize())
              .correct_position(true)
              .set_border_color(sf::Color::Black)
              .set_border_width(2)
              .to_center(Element::XY)
              .move(Element::Y, -(WINDOW_H / 4))
              .build();

            start_button.button_text_to_center();

            Button score_button { start_button };
            score_button
              .set_text("Счёт")
              .set_after(Element::Y, start_button)
              .move(Element::Y, 30)
              .build();

            score_button.button_text_to_center();

            Button help_button { start_button };
            help_button
              .set_text("Помощь")
              .set_after(Element::Y, score_button)
              .move(Element::Y, 30)
              .build();

            help_button.button_text_to_center();

            Button exit_button { start_button };
            exit_button
              .set_text("Выход")
              .set_after(Element::Y, help_button)
              .move(Element::Y, 30)
              .build();

            exit_button.button_text_to_center();

            for(auto& button: vector<Button*> { &start_button, &score_button, &exit_button, &help_button }) {
                button->on_hover(
                  [](Element& el) {
                      el
                       .set_border_width(3)
                       .build();
                  },
                  [](Element& el) {
                      el
                       .set_border_width(2)
                       .build();
                  }
                );

                button->on_click(
                  [](Element& el) {
                       auto& button = dynamic_cast<Button&>(el);

                       button
                         .set_bg_color(sf::Color::Black)
                         .set_text_color(sf::Color(192,192,192))
                         .build();
                  },
                  [&](Element& el) {
                      auto& butn = dynamic_cast<Button&>(el);

                      butn
                        .set_bg_color(sf::Color(192,192,192))
                        .set_text_color(sf::Color::Black)
                        .build();

                      if(&el == &start_button) {
                          go_start = true;
                      }
                      if(&el == &score_button) {
                          go_scores = true;
                      }
                      if(&el == &help_button) {
                          go_help = true;
                      }
                      if(&el == &exit_button) {
                          go_exit = true;
                      }
                  }
                );
            }

            while (window.isOpen()) {
                /// EVENTS
                Event event {};
                while (window.pollEvent(event)) {
                    if (event.type == Event::Closed) {
                        window.close();
                        return false;
                    }

                    data_utils::MousePosition::input(mouse_position, window, event);

                    Button::input(start_button, window, event, mouse_position.get_prev_pos());
                    Button::input(score_button, window, event, mouse_position.get_prev_pos());
                    Button::input(help_button, window, event, mouse_position.get_prev_pos());
                    Button::input(exit_button, window, event, mouse_position.get_prev_pos());
                }

                /// DRAW
                window.clear(sf::Color(192, 192, 192));

                start_button.draw(window);
                score_button.draw(window);
                help_button.draw(window);
                exit_button.draw(window);

                username_text.draw(window);
                username.draw(window);

                window.display();

                if(go_help) {
                    strategies.next(6);
                    break;
                }
                if(go_start) {
                    strategies.next(4);
                    break;
                }
                if(go_scores) {
                    strategies.next(3);
                    break;
                }
                if(go_exit) {
                    strategies.stop();
                    break;
                }
            }

            return true;
        });

        // 4. user scores
        strategies.register_strategy([&]() -> bool {
            size_t count_of_scores = std::distance(current_user.child("scores").children().begin(), current_user.child("scores").children().end());
            size_t offset = 0;

            bool go_back = false;

            int rect_w = 500;
            int rect_h = 250;
            RectangleShape rectangle(Vector2f(rect_w, rect_h));
            rectangle.setOutlineThickness(2);
            rectangle.setOutlineColor(sf::Color::Black);
            rectangle.setFillColor(sf::Color(192,192,192));
            rectangle.setPosition({
                (WINDOW_W / 2.f) - (rect_w / 2.f),
                (WINDOW_H / 2.f) - (rect_h / 2.f)
            });

            const auto& rect_bounds = rectangle.getGlobalBounds();

            int rect_scroll_w = 20;
            int rect_scroll_h = count_of_scores > 0 ? int(rect_h / count_of_scores) : rect_h;
            RectangleShape rect_scroll(Vector2f(rect_scroll_w, rect_scroll_h));
            rect_scroll.setFillColor(sf::Color::Black);
            rect_scroll.setPosition({
               rect_bounds.left + rect_bounds.width + 5,
               rect_bounds.top + 2
            });

            form::types::Text username_text { "Имя игрока: " };
            username_text
              .set_text_size(16)
              .set_color(sf::Color::Black)
              .set_window_size(window.getSize())
              .correct_position(true)
              .move(Element::Y, 10)
              .to(Element::Angle::Right)
              .move(Element::X, - 60)
              .build();

            form::types::Text username { username_text };
            username
              .set_text(g_username)
              .set_color(sf::Color::Red)
              .set_after(Element::X, username_text)
              .build();

            form::types::Text result_text { "Результаты: " };
            result_text
              .set_text_size(18)
              .set_color(sf::Color::Black)
              .set_window_size(window.getSize())
              .correct_position(true)
              .move(Element::Y, 50)
              .move(Element::X, 47)
              .build();

            form::types::Text count_of_scores_text { result_text };
            count_of_scores_text
              .set_text(to_string(count_of_scores > 0 ? offset + 1 : offset) + " / " + to_string(count_of_scores))
              .move(Element::X, 425)
              .build();

            Button back_button {{100, 40 }, "Назад" };
            back_button
              .set_text_size(20)
              .set_bg_color(sf::Color(192,192,192))
              .set_window_size(window.getSize())
              .correct_position(true)
              .set_border_color(sf::Color::Black)
              .set_border_width(2)
              .to_center(Element::X)
              .move(Element::Y, 345)
              .build();

            back_button.button_text_to_center();

            back_button.on_hover(
              [](Element& el) {
                 el
                  .set_border_width(3)
                  .build();
              },
              [](Element& el) {
                 el
                  .set_border_width(2)
                  .build();
              }
            );

            back_button.on_click(
             [](Element& el) {
                auto& button = dynamic_cast<Button&>(el);

                button
                  .set_bg_color(sf::Color::Black)
                  .set_text_color(sf::Color(192,192,192))
                  .build();
                },
             [&](Element& el) {
                auto& button = dynamic_cast<Button&>(el);

                 button
                  .set_bg_color(sf::Color(192,192,192))
                  .set_text_color(sf::Color::Black)
                  .build();

                  go_back = true;
                }
             );

            sf::Text score_time_text {};
            score_time_text.setCharacterSize(18);
            score_time_text.setFillColor(sf::Color::Black);
            score_time_text.setFont(engine::Engine::get_game_font());
            score_time_text.setPosition(
               rect_bounds.left + 20,
               rect_bounds.top + 20
            );

            sf::Text score_detail_text { score_time_text };
            score_detail_text.setPosition(
               score_time_text.getPosition().x + 20,
               score_time_text.getPosition().y + 30
            );

            while (window.isOpen()) {
                /// EVENTS
                Event event {};
                while (window.pollEvent(event)) {
                    if (event.type == Event::Closed) {
                        window.close();
                        return false;
                    }

                    if(event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                        if(std::abs(event.mouseWheelScroll.delta) <= 1) {
                            auto next_offset = offset - event.mouseWheelScroll.delta;
                            if(next_offset >= 0 && next_offset < count_of_scores) {
                                offset = next_offset;
                            }
                        }
                    }

                    data_utils::MousePosition::input(mouse_position, window, event);
                    Button::input(back_button, window, event, mouse_position.get_prev_pos());
                }

                /// DRAW
                window.clear(sf::Color(192, 192, 192));

                rect_scroll.setPosition(
                    rect_scroll.getPosition().x,
                    rect_bounds.top + 2 + (offset * rect_scroll_h)
                );

                window.draw(rectangle);
                window.draw(rect_scroll);

                if(count_of_scores > 0) {
                    auto score_it = std::end(current_user.child("scores").children());
                    score_it--;

                    for(int i = 0; i < offset; i++) {
                        score_it--;
                    }

                    auto score_node = *score_it;

                    time_t tms = score_node.attribute("tms").as_llong();
                    bool victory = stoi(score_node.child_value("victory"));

                    std::stringstream buffer_time;
                    buffer_time << std::put_time(localtime(&tms), "%d/%m/%Y %H:%M:%S");

                    std::wstringstream buffer_detail;
                    buffer_detail
                      << L"Уровень: " << score_node.child_value("level") << endl
                      << L"Искомая комната: " << score_node.child_value("door") << endl
                      << L"Потраченое время: " << score_node.child_value("time") << L" сек." << endl
                      << L"Колличество шагов: " << score_node.child_value("steps") << endl
                      << L"Победа: " << (victory ? L"Да" : L"Нет") << endl;

                    score_time_text.setString(String(buffer_time.str()));
                    score_detail_text.setString(String(buffer_detail.str()));

                    window.draw(score_time_text);
                    window.draw(score_detail_text);
                }

                count_of_scores_text
                  .set_text(to_string(count_of_scores > 0 ? offset + 1 : offset) + " / " + to_string(count_of_scores))
                  .build();
                count_of_scores_text.draw(window);

                result_text.draw(window);
                username_text.draw(window);
                username.draw(window);

                back_button.draw(window);

                window.display();

                if(go_back) {
                    strategies.next(2);
                    break;
                }
            }

            return true;
        });

        // 5. select level
        strategies.register_strategy([&]() -> bool {
            bool go_game = false;
            bool go_back = false;

            form::types::Text select_level_text { "Выберите уровень" };
            select_level_text
              .set_text_size(26)
              .set_color(sf::Color::Black)
              .set_window_size(window.getSize())
              .correct_position(true)
              .to_center(Element::X)
              .move(Element::X, -20)
              .move(Element::Y, 30)
              .build();

            Button level1_button {{110, 100 }, "" };
            level1_button
              .set_bg_color(sf::Color(192,192,192))
              .set_window_size(window.getSize())
              .correct_position(true)
              .set_border_color(sf::Color::Blue)
              .set_border_width(2)
              .move(Element::X, 23)
              .set_after(Element::Y, select_level_text)
              .move(Element::Y, 33)
              .build();

            Button level2_button { level1_button };
            level2_button
              .set_after(Element::X, level1_button)
              .move(Element::X, 40)
              .build();

            Texture t_level1, t_level2;
            t_level1.loadFromFile(File::resolve_path(DIR_PATH, "./data/textures/level1.jpeg"));
            t_level2.loadFromFile(File::resolve_path(DIR_PATH, "./data/textures/level2.jpeg"));

            sf::Text level1_text;
            level1_text.setCharacterSize(22);
            level1_text.setFillColor(sf::Color::Black);
            level1_text.setFont(engine::Engine::get_game_font());
            level1_text.setString(L"Уровень 1");
            level1_text.setPosition(30, select_level_text.get_y() + 50);

            sf::Text level2_text(level1_text);
            level2_text.setString(L"Уровень 2");
            level2_text.setPosition(180, select_level_text.get_y() + 50);

            Sprite s_level1, s_level2;
            s_level1.setTexture(t_level1);
            s_level1.setPosition(
               level1_text.getPosition().x + (level1_text.getGlobalBounds().width / 2.f) - (s_level1.getGlobalBounds().width / 2.f),
               level1_text.getPosition().y + 30
            );

            s_level2.setTexture(t_level2);
            s_level2.setPosition(
               level2_text.getPosition().x + (level1_text.getGlobalBounds().width / 2.f) - (s_level1.getGlobalBounds().width / 2.f),
               level2_text.getPosition().y + 30
            );

            Button back_button {{100, 40 }, "Назад" };
            back_button
              .set_text_size(20)
              .set_bg_color(sf::Color(192,192,192))
              .set_window_size(window.getSize())
              .correct_position(true)
              .set_border_color(sf::Color::Black)
              .set_border_width(2)
              .to_center(Element::X)
              .move(Element::Y, 345)
              .build();

            back_button.button_text_to_center();

            back_button.on_hover(
              [](Element& el) {
                 el
                  .set_border_width(3)
                  .build();
              },
              [](Element& el) {
                 el
                  .set_border_width(2)
                  .build();
              }
            );

            back_button.on_click(
             [](Element& el) {
                auto& button = dynamic_cast<Button&>(el);

                button
                  .set_bg_color(sf::Color::Black)
                  .set_text_color(sf::Color(192,192,192))
                  .build();
                },
             [&](Element& el) {
                auto& button = dynamic_cast<Button&>(el);

                 button
                  .set_bg_color(sf::Color(192,192,192))
                  .set_text_color(sf::Color::Black)
                  .build();

                  go_back = true;
                }
             );

            for(auto& button: vector<Button*> { &level1_button, &level2_button }) {
                button->on_hover(
                  [](Element& el) {
                      auto& button = dynamic_cast<Button&>(el);

                      button
                       .set_bg_color(sf::Color::Blue)
                       .build();
                  },
                  [](Element& el) {
                      auto& button = dynamic_cast<Button&>(el);

                      button
                       .set_bg_color(sf::Color(192,192,192))
                       .build();
                  }
                );

                button->on_click(
                  [](Element& el) {
                      el
                       .set_border_width(4)
                       .build();
                  },
                  [&](Element& el) {
                      el
                        .set_border_width(2)
                        .build();

                      if(&el == &level1_button) {
                          g_selected_level = 1;
                          go_game = true;
                      }
                      if(&el == &level2_button) {
                          g_selected_level = 2;
                          go_game = true;
                      }
                  }
                );
            }

            while (window.isOpen()) {
                /// EVENTS
                Event event {};
                while (window.pollEvent(event)) {
                    if (event.type == Event::Closed) {
                        window.close();
                        return false;
                    }

                    data_utils::MousePosition::input(mouse_position, window, event);
                    Button::input(back_button, window, event, mouse_position.get_prev_pos());
                    Button::input(level1_button, window, event, mouse_position.get_prev_pos());
                    Button::input(level2_button, window, event, mouse_position.get_prev_pos());
                }

                /// DRAW
                window.clear(sf::Color(192, 192, 192));

                level1_button.draw(window);
                level2_button.draw(window);

                window.draw(level1_text);
                window.draw(level2_text);

                window.draw(s_level1);
                window.draw(s_level2);

                select_level_text.draw(window);
                back_button.draw(window);

                window.display();

                if(go_game) {
                    strategies.next(5);
                    break;
                }
                if(go_back) {
                    strategies.next(2);
                    break;
                }
            }

            return true;
        });

        // 6. game
        strategies.register_strategy([&]() -> bool {
            if(g_selected_level == -1) {
                strategies.next(4);
                return true;
            }

            const int BLOCK_SIZE = 14;
            const int MINI_BLOCK_SIZE = 4;
            const int MAX_BLOCK_VISING_X = 24;
            const int MAX_BLOCK_VISING_Y = 20;

            const time_t start_game_time = time(0);

            bool go_exit = false;
            bool game_stop = false;

            bool room_found = false;
            bool wrong_door_is_open = false;
            bool player_loose = false;
            bool can_change_floor = false;
            bool can_open_door = false;

            auto key_z_pressed = false;
            auto key_x_pressed = false;

            auto player_score = current_user.child("scores").append_child("score");
            // XML: save tms
            player_score.append_attribute("tms").set_value(time(0));

            auto level_path = level_paths.at(g_selected_level);
            std::cout << "level_path = " << level_path << std::endl;

            FileReader f_r(level_path);
            const json level_data = f_r.to_json();
            const json& level_map = level_data["map"];

            int current_floor = utils::array::gen_random(0, level_map.size() - 1);
            int player_lives = 3;

            string need_find_door {};
            {
                vector<string> all_doors;
                for(const auto& lvl_map : level_map) {
                    if(lvl_map["doors"].is_object()) {
                        for(const auto& el: lvl_map["doors"]) {
                            all_doors.push_back(to_string(el));
                        }
                    }
                }

                need_find_door = all_doors.at(utils::array::gen_random(0, all_doors.size() - 1));
                need_find_door.replace(0, 1, "");
                need_find_door.replace(need_find_door.size() - 1, need_find_door.size(), "");
            }

            json current_map = level_map[current_floor];
            string* tail_map = utils::string::json_arr_to_string(current_map["tail"], current_map["height"]);

            form::types::Text door_text { "Нажмите [x] что бы открыть дверь." };
            door_text
              .set_color(sf::Color::White)
              .set_text_size(22)
              .set_window_size(window.getSize())
              .correct_position(true)
              .to_center(Element::XY)
              .to(Element::Angle::Down)
              .move(Element::Y, -20)
              .build();

            form::types::Text floor_text { door_text };
            floor_text
              .set_text(string("Нажмите [z] что бы сменить этаж."))
              .build();

            const string map_level_str = "Текущий этаж: ";
            form::types::Text map_level_text { map_level_str + "0" };
            map_level_text
              .set_color(sf::Color::White)
              .set_text_size(14)
              .set_window_size(window.getSize())
              .correct_position(true)
              .move(Element::Y, int(current_map["height"]) * MINI_BLOCK_SIZE + 10)
              .move(Element::X, 5)
              .build();

            const string need_door_str = "Дверь: " + need_find_door;
            form::types::Text need_door_text { need_door_str };
            need_door_text
              .set_color(sf::Color::White)
              .set_text_size(20)
              .set_window_size(window.getSize())
              .correct_position(true)
              .to_center(Element::X)
              .move(Element::Y, 5)
              .build();

            const string player_count_steps_str = "Колличество шагов: ";
            form::types::Text player_count_steps_text { player_count_steps_str + "0" };
            player_count_steps_text
             .set_color(sf::Color::White)
             .set_text_size(14)
             .set_window_size(window.getSize())
             .correct_position(true)
             .to(Element::Angle::Right)
             .move(Element::X, 25)
             .move(Element::Y, 20)
             .build();

            const string player_attempts_str = "Колличество попыток: ";
            form::types::Text player_attempts_text { player_count_steps_text };
            player_attempts_text
              .set_text(player_attempts_str + to_string(player_lives))
              .set_before(Element::Y, player_count_steps_text)
              .move(Element::Y, -5)
              .build();

            const string victory_game_str = "Вы выиграли";
            form::types::Text victory_game_text { victory_game_str };
            victory_game_text
              .set_color(sf::Color::White)
              .set_text_size(22)
              .set_window_size(window.getSize())
              .correct_position(true)
              .to_center(Element::XY)
              .move(Element::Y, -(100 / 2.f) + 20)
              .move(Element::X, -10)
              .build();

            const string wrong_door_open_str = "Это не та дверь!";
            form::types::Text wrong_door_open_text { victory_game_text };
            wrong_door_open_text
              .set_color(sf::Color::Black)
              .set_text(wrong_door_open_str)
              .build();

            const string loose_game_str = "Вы проиграли";
            form::types::Text loose_game_text { victory_game_text };
            loose_game_text
              .set_text(loose_game_str)
              .build();

            const string stop_game_str = "СТОП";
            form::types::Text stop_game_text { stop_game_str };
            stop_game_text
              .set_text_size(38)
              .set_window_size(window.getSize())
              .correct_position(true)
              .to_center(Element::XY)
              .build();

            Button continue_button { {120, 30}, "Продолжить" };
            continue_button
             .set_bg_color(sf::Color(192,192,192))
             .set_window_size(window.getSize())
             .correct_position(true)
             .set_border_color(sf::Color::Black)
             .set_border_width(2)
             .set_after(Element::Y, stop_game_text)
             .move(Element::Y, 50)
             .set_after(Element::X, stop_game_text)
             .move(Element::X, -(continue_button.get_width() / 2.f))
             .build();

            continue_button.button_text_to_center();

            Button exit_button { continue_button };
            exit_button
             .set_text("Выход")
             .set_after(Element::Y, continue_button)
             .move(Element::Y, 15)
             .build();

            exit_button.button_text_to_center();

            for(auto& button: vector<Button*> { &continue_button, &exit_button }) {
                 button->on_click(
                  [](Element& el) {},
                  [&wrong_door_is_open, &game_stop, &continue_button, &exit_button, &go_exit](Element& el) {
                      if(&el == &continue_button) {
                          if(game_stop) game_stop = false;

                          if(wrong_door_is_open) wrong_door_is_open = false;
                      }

                      if(&el == &exit_button) {
                          go_exit = true;
                      }
                  }
                );

                 button->on_hover(
                    [](Element& el) {
                        el
                         .set_border_width(3)
                         .build();
                    },
                    [](Element& el) {
                        el
                         .set_border_width(2)
                         .build();
                    }
                );
            }

            GameMap map(BLOCK_SIZE);
            map.set_windows_size(window.getSize());
            map.load_tile(tail_map, current_map["width"], current_map["height"]);
            map.set_paddings({
               0,
               (WINDOW_H / 2) - ((int(current_map["height"]) * BLOCK_SIZE) / 2)
            });
            map.register_collision_cells("BZD");

            GameMap mini_map(MINI_BLOCK_SIZE);
            mini_map.set_windows_size(window.getSize());
            mini_map.load_tile(tail_map, current_map["width"], current_map["height"]);
            mini_map.set_max_block_vising_x(MAX_BLOCK_VISING_X);
            mini_map.set_max_block_vising_y(MAX_BLOCK_VISING_Y);
            mini_map.set_paddings({ 10, 5 });
            mini_map.user_border(3, 3, sf::Color::Green);

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
                player1.set_position(x * map.get_block_size(), y * map.get_block_size());
                map.calculate_offset(player1.get_position());
            };

            const auto& check_found_door = [&](const string& door) {
                if(door == need_find_door) {
                    room_found = true;
                    game_stop = true;

                    exit_button
                      .set_size({ 100, 30 })
                      .set_border_color(sf::Color::White)
                      .to_center(Element::X)
                      .set_after(Element::Y, victory_game_text)
                      .move(Element::Y, 25)
                      .to_center(Element::X)
                      .build();

                    exit_button.button_text_to_center();
                } else {
                    player_lives--;
                    wrong_door_is_open = true;

                    if(player_lives == 0) {
                        game_stop = true;
                        player_loose = true;

                        exit_button
                          .set_size({ 100, 30 })
                          .set_border_color(sf::Color::White)
                          .set_after(Element::Y, victory_game_text)
                          .move(Element::Y, 25)
                          .to_center(Element::X)
                          .build();

                        exit_button.button_text_to_center();
                    }
                }
            };

            const auto& save_player_result = [&]() -> void {
                // XML: save level
                player_score
                  .append_child("level")
                  .append_child(pugi::node_pcdata)
                  .set_value(to_string(g_selected_level).c_str());

                // XML: save door
                player_score
                  .append_child("door")
                  .append_child(pugi::node_pcdata)
                  .set_value(need_find_door.c_str());

                // XML: save time
                player_score
                  .append_child("time")
                  .append_child(pugi::node_pcdata)
                  .set_value(to_string(time(0) - start_game_time).c_str());

                // XML: save steps
                player_score
                  .append_child("steps")
                  .append_child(pugi::node_pcdata)
                  .set_value(to_string(int(player1.get_steps_count() / 2)).c_str());

                // XML: save victory
                player_score
                  .append_child("victory")
                  .append_child(pugi::node_pcdata)
                  .set_value(to_string((int)room_found).c_str());

                // XML: save result
                xml_doc.save_file(File::resolve_path(DIR_PATH, "./data/xml/users.xml").c_str());
            };

            RectangleShape foreground(Vector2f(WINDOW_W, WINDOW_H));
            foreground.setFillColor(sf::Color(192,192,192, 128));

            float game_result_rect_w = 300;
            float game_result_rect_h = 100;
            RectangleShape game_result_rect(Vector2f(game_result_rect_w, game_result_rect_h));
            game_result_rect.setFillColor(sf::Color::Black);
            game_result_rect.setPosition(
               (WINDOW_W / 2.f) - (game_result_rect_w / 2.f),
               (WINDOW_H / 2.f) - (game_result_rect_h / 2.f)
            );

            RectangleShape rectangle(Vector2f(BLOCK_SIZE, BLOCK_SIZE));
            rectangle.setFillColor(Color::White);

            RectangleShape mini_rectangle(Vector2f(MINI_BLOCK_SIZE, MINI_BLOCK_SIZE));

            {
                mini_map.register_draw_element(' ', mini_rectangle, [](sf::RectangleShape* rect) -> void {
                    rect->setFillColor(sf::Color::White);
                });
                mini_map.register_draw_element('B', mini_rectangle, [](sf::RectangleShape* rect) -> void {
                    rect->setFillColor(sf::Color(192, 192, 192));
                });
                mini_map.register_draw_element('Z', mini_rectangle, [](sf::RectangleShape* rect) -> void {
                    rect->setFillColor(sf::Color::Yellow);
                });
                mini_map.register_draw_element('D', mini_rectangle, [](sf::RectangleShape* rect) -> void {
                    rect->setFillColor(sf::Color::Cyan);
                });
            }

            {
                map.register_draw_element(' ', rectangle, [](sf::RectangleShape* rect) -> void {
                    rect->setFillColor(sf::Color::White);
                });
                map.register_draw_element('B', rectangle, [](sf::RectangleShape* rect) -> void {
                    rect->setFillColor(sf::Color(192, 192, 192));
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

            Clock clock;
            while (window.isOpen()) {
                float time = clock.getElapsedTime().asMicroseconds();
                clock.restart();

                time = time / 600;

                can_open_door = false;
                can_change_floor = false;

                key_z_pressed = false;
                key_x_pressed = false;

                /// EVENTS
                Event event {};
                while (window.pollEvent(event)) {
                    if (event.type == Event::Closed) {
                        // XML: remove incomplete data
                        current_user.child("scores").remove_child(player_score);
                        // XML: save result
                        xml_doc.save_file(File::resolve_path(DIR_PATH, "./data/xml/users.xml").c_str());

                        window.close();
                    }

                    if(event.type == sf::Event::KeyPressed) {
                        if(event.key.code == sf::Keyboard::Escape) {
                            if(!player_loose && !room_found) game_stop = !game_stop;
                        }

                        if(event.key.code == sf::Keyboard::Z) {
                            key_z_pressed = true;
                        }

                        if(event.key.code == sf::Keyboard::X) {
                            key_x_pressed = true;
                        }
                    }

                    if(!game_stop) player1.run(event.key.shift);

                    data_utils::MousePosition::input(mouse_position, window, event);

                    Button::input(continue_button, window, event, mouse_position.get_prev_pos());
                    Button::input(exit_button, window, event, mouse_position.get_prev_pos());
                }

                /// GAME
                if(!game_stop) {
                    Player::move_input(player1);
                    player1.update(time);
                }

                string last_door_key {};
                player1.around_blocks('Z', [&](auto& block) -> void {
                    auto last_axis = GameMap::Axis::X;
                    auto door_range = map.find_cell_sequence(block, GameMap::Axis::X);

                    if(door_range.get_start() == door_range.get_end()) {
                        door_range = map.find_cell_sequence(block, last_axis = GameMap::Axis::Y);
                    }

                    last_door_key = door_range.to_string(last_axis);
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

                if(key_x_pressed && can_open_door) {
                    const auto& door = current_map["doors"][last_door_key];

                    if(!door.is_null()) {
                        check_found_door(door);
                    }
                }

                if(go_exit) {
                    save_player_result();

                    strategies.next(4);
                    break;
                }

                /// DRAW
                window.clear(Color::Black);

                map.draw(window);
                mini_map.draw(window, &player_on_map);

                if(can_open_door) {
                    door_text.draw(window);
                }
                if(can_change_floor) {
                    floor_text.draw(window);
                }

                need_door_text.draw(window);

                map_level_text
                  .set_text(map_level_str + std::to_string(current_floor + 1))
                  .build();
                map_level_text.draw(window);

                player_count_steps_text
                  .set_text(player_count_steps_str + std::to_string(int(player1.get_steps_count() / 2)))
                  .build();
                player_count_steps_text.draw(window);

                player_attempts_text
                  .set_text(player_attempts_str + to_string(player_lives))
                  .build();;
                player_attempts_text.draw(window);

                window.draw(player1.get_sprite());

                if(room_found && game_stop) {
                    window.draw(foreground);
                    window.draw(game_result_rect);

                    victory_game_text.draw(window);
                    exit_button.draw(window);
                } else if(player_loose && game_stop) {
                    window.draw(foreground);
                    window.draw(game_result_rect);

                    loose_game_text.draw(window);
                    exit_button.draw(window);
                } else if(game_stop) {
                    window.draw(foreground);
                    stop_game_text.draw(window);

                    continue_button.draw(window);
                    exit_button.draw(window);
                }

                if(wrong_door_is_open && !game_stop && !player_loose) {
                    window.draw(foreground);
                    wrong_door_open_text.draw(window);

                    continue_button.draw(window);
                }

                window.display();
            }

            delete[] tail_map;

            return true;
        });

        // 7. help
        strategies.register_strategy([&]() -> bool {
            bool go_back = false;

            int rect_w = 500;
            int rect_h = 250;
            RectangleShape rectangle(Vector2f(rect_w, rect_h));
            rectangle.setOutlineThickness(2);
            rectangle.setOutlineColor(sf::Color::Black);
            rectangle.setFillColor(sf::Color(192,192,192));
            rectangle.setPosition({
                (WINDOW_W / 2.f) - (rect_w / 2.f),
                (WINDOW_H / 2.f) - (rect_h / 2.f)
            });

            const auto& rect_bounds = rectangle.getGlobalBounds();

            form::types::Text help_text { "Помощь" };
            help_text
              .set_text_size(28)
              .set_color(sf::Color::Black)
              .set_window_size(window.getSize())
              .correct_position(true)
              .to_center(Element::X)
              .move(Element::X, -10)
              .move(Element::Y, 20)
              .build();

            form::types::Text prompt1_text { "1. Нажмите 'ESC' во время игры что бы поставить игру на паузу." };
            prompt1_text
              .set_text_size(16)
              .set_color(sf::Color::Black)
              .set_window_size(window.getSize())
              .correct_position(true)
              .move(Element::X, rect_bounds.left + 20)
              .move(Element::Y, rect_bounds.top + 20)
              .build();

            form::types::Text prompt2_text { prompt1_text };
            prompt2_text
              .set_text(string("2.        - это дверь."))
              .set_after(Element::Y, prompt1_text)
              .move(Element::Y, 15)
              .build();

            form::types::Text prompt3_text { prompt2_text };
            prompt3_text
              .set_text(string("3.        - это лестница на другой этаж."))
              .set_after(Element::Y, prompt2_text)
              .move(Element::Y, 15)
              .build();

            form::types::Text prompt4_text { prompt3_text };
            prompt4_text
              .set_text(string("4. Розработчики: Золотаренко Д. и Мыкал М. и Пахомов Р."))
              .set_after(Element::Y, prompt3_text)
              .move(Element::Y, 15)
              .build();

            RectangleShape door_rect(Vector2f(15, 15));
            door_rect.setFillColor(sf::Color::Yellow);
            door_rect.setPosition({
                rect_bounds.left + 38,
                rect_bounds.top + 55
            });

            RectangleShape floor_rect(Vector2f(15, 15));
            floor_rect.setFillColor(sf::Color::Cyan);
            floor_rect.setPosition({
                rect_bounds.left + 38,
                rect_bounds.top + 85
            });

            Button back_button {{100, 40 }, "Назад" };
            back_button
              .set_text_size(20)
              .set_bg_color(sf::Color(192,192,192))
              .set_window_size(window.getSize())
              .correct_position(true)
              .set_border_color(sf::Color::Black)
              .set_border_width(2)
              .to_center(Element::X)
              .move(Element::Y, 345)
              .build();

            back_button.button_text_to_center();

            back_button.on_hover(
              [](Element& el) {
                 el
                  .set_border_width(3)
                  .build();
              },
              [](Element& el) {
                 el
                  .set_border_width(2)
                  .build();
              }
            );

            back_button.on_click(
             [](Element& el) {
                auto& button = dynamic_cast<Button&>(el);

                button
                  .set_bg_color(sf::Color::Black)
                  .set_text_color(sf::Color(192,192,192))
                  .build();
                },
             [&](Element& el) {
                auto& button = dynamic_cast<Button&>(el);

                 button
                  .set_bg_color(sf::Color(192,192,192))
                  .set_text_color(sf::Color::Black)
                  .build();

                  go_back = true;
                }
             );

            while (window.isOpen()) {
                /// EVENTS
                Event event {};
                while (window.pollEvent(event)) {
                    if (event.type == Event::Closed) {
                        window.close();
                        return false;
                    }

                    data_utils::MousePosition::input(mouse_position, window, event);
                    Button::input(back_button, window, event, mouse_position.get_prev_pos());
                }

                /// DRAW
                window.clear(sf::Color(192, 192, 192));

                window.draw(rectangle);

                prompt1_text.draw(window);
                prompt2_text.draw(window);
                prompt3_text.draw(window);
                prompt4_text.draw(window);

                window.draw(door_rect);
                window.draw(floor_rect);

                help_text.draw(window);
                back_button.draw(window);

                window.display();

                if(go_back) {
                    strategies.next(2);
                    break;
                }
            }

            return true;
        });

        strategies.next(5); // TODO _back
        strategies.start(); // TODO _back

        xml_doc.save_file("/home/user/Code/stud-game/data/xml/users.xml");

        data_utils::MousePosition::remove_instance(&mouse_position);

        controllers::FocusController::remove_instance();
        controllers::CallSchedulerController::remove_instance();

    } catch (const Exception& ex) {
        ex.draw_error();
    } catch (const std::exception &ex) {
        cout << "error = " << ex.what() << endl;
    } catch (...) {
        cout << "Unknown error = " << endl;
    }

    return EXIT_SUCCESS;
}