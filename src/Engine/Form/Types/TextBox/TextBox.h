//
// Created by user on 07.04.2021.
//

#ifndef STUD_GAME_TEXTBOX_H
#define STUD_GAME_TEXTBOX_H

#include <functional>
#include <vector>

#include "../Button/Button.h"
#include "../Text/Text.h"

#include "../../Objects/FocusObject/FocusObject.h"

class TextBox: public Button {
    protected:
        uint8_t m_max_characters_number = 100;

    public:
        explicit TextBox(const sf::Vector2f& size, const string& str): Button(size, str) {};
        explicit TextBox(const sf::Vector2f& size, const sf::Vector2f& pos, const string& str): Button(size, pos, str) {};

        virtual TextBox& build() override;

        TextBox& set_max_chars_number(uint8_t num);

        string get_entered_text() const;
        char32_t operator[] (size_t index);

        static void input(TextBox& text_box, const sf::RenderWindow& window, const sf::Event& event);
};


#endif //STUD_GAME_TEXTBOX_H
