//
// Created by user on 09.02.2021.
//

#ifndef STUD_GAME_BUTTON_H
#define STUD_GAME_BUTTON_H

#include <string>
#include <vector>     // vector
#include <functional> // function

#include <SFML/Graphics.hpp>
#include <utility>

#include "../../../Exception/Exception.h"

#include "../../ExtendedTypes/ActionElement/ActionElement.h"
#include "../../Types/Text/Text.h"

#include "../../../Engine.h"
#include "../../Form.h"

using namespace std;

class Button: public ActionElement {
    protected:
        sf::Color m_bg_color { sf::Color::White };

        Text *m_text { nullptr };
        sf::RectangleShape *m_rectangle { nullptr };

        static void handle_mouse_button_pressed_e(Button& button, const sf::RenderWindow& window);
        static void handle_mouse_button_released_e(Button& button, const sf::RenderWindow& window);
        static void handle_mouse_moved_e(Button& button, const sf::RenderWindow& window, const sf::Vector2i& prev_pos);

    public:
        explicit Button(const sf::Vector2f& size, const string& str);
        explicit Button(const sf::Vector2f& size, const sf::Vector2f& pos, const string& str);
        ~Button() override;

        virtual Button& build() override;
        virtual void draw(sf::RenderWindow& window) const override;

        Button& set_window_size(const sf::Vector2u& size, bool correct_pos = false) override;

        Button& add_text(const string& text);
        Button& narrow_text(size_t start_count, size_t end_count = 0);

        Button& set_text(const string& text);
        Button& set_text_size(int size);
        Button& set_text_color(const sf::Color& color);

        Button& set_bg_color(const sf::Color& color);

        Button& button_text_to_center();

        Text& get_button_text() const;

        static void input(Button& button, const sf::RenderWindow& window, const sf::Event& event, const sf::Vector2i& prev_pos);
};

#endif //STUD_GAME_BUTTON_H
