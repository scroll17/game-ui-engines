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
        bool m_text_changed;
        Text m_text {""};

    public:
        explicit TextBox(const sf::Vector2f& size, const string& str): Button(size, str) {};
        explicit TextBox(const sf::Vector2f& size, const sf::Vector2f& pos, const string& str): Button(size, pos, str) {};

        virtual TextBox& set_focus() override;
        virtual TextBox& delete_focus() override;

//        virtual TextBox& build() override;
//        virtual void draw(sf::RenderWindow& window) const override;
//
//        void write_text(char ch);
//        const string& read_text() const;
//
        static void input(TextBox& text_box, const sf::RenderWindow& window, const sf::Event& event);
};


#endif //STUD_GAME_TEXTBOX_H
