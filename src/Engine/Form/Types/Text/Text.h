//
// Created by user on 13.02.2021.
//

#ifndef STUD_GAME_TEXT_H
#define STUD_GAME_TEXT_H

#include <string>
#include <SFML/Graphics.hpp>

#include "../Element/Element.h"
#include "../../../Engine.h"

using namespace std;

namespace form {
    namespace types {

        class Button;
        class TextBox;

        class Text: public Element {
            friend class Button;
            friend class TextBox;

            public:
                enum ActionPosition: int8_t {
                    Current,
                    Before,
                    After
                };

            protected:
                int m_text_size { 18 };

                sf::Font& m_font { engine::Engine::get_game_font() };
                sf::Color m_color { sf::Color::Black };

                sf::Text *m_value { nullptr };

                void init(const string& value);
                void actualize_size();
                void actualize_width();
                void actualize_height();

            public:
                explicit Text(const string& value);
                explicit Text(const string& value, sf::Font& font, sf::Color& color);
                explicit Text(const Text& text);
                ~Text() override;

                virtual Text& build() override;
                virtual void draw(sf::RenderWindow& window) const override;

                Text& add_char(char32_t ch, const ActionPosition& action = ActionPosition::Current, size_t pos = 0);
                Text& add_text(const sf::String& str, const ActionPosition& action = ActionPosition::Current, size_t pos = 0);
                Text& add_text(const string& str, const ActionPosition& action = ActionPosition::Current, size_t pos = 0);

                Text& remove_char(size_t pos);
                Text& remove_chars(size_t pos, size_t count);

                Text& narrow_text(size_t start_count, size_t end_count = 0);

                Text& set_text(const string& str);
                Text& set_text(const sf::String& str);
                Text& set_text_size(int size);

                Text& set_font(const sf::Font& font);
                Text& set_color(const sf::Color& color);

                string get_value() const;
                size_t get_size() const;

                bool is_empty() const;

                char32_t operator[] (size_t index);
        };

    }
}

#endif //STUD_GAME_TEXT_H
