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

class Text: public Element {
    private:
        using Element::set_size;
        using Element::set_width;
        using Element::set_height;

    protected:
        int m_text_size { 18 };

        sf::Font& m_font {Engine::get_game_font() };
        sf::Color m_color { sf::Color::Black };

        sf::Text *m_value { nullptr };

        void init(const string& value);
        void actualize_size();

    public:
        explicit Text(const string& value);
        explicit Text(const string& value, sf::Font& font, sf::Color& color);
        ~Text() override;

        virtual Text& build() override;
        virtual void draw(sf::RenderWindow& window) const override;

        Text& set_text(const string& str);
        Text& set_text_size(int size);

        Text& set_font(const sf::Font& font);
        Text& set_color(const sf::Color& color);
};


#endif //STUD_GAME_TEXT_H
