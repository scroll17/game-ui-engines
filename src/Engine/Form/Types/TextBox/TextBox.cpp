//
// Created by user on 07.04.2021.
//

#include "TextBox.h"

using namespace form::types;

// PUBLIC SET
TextBox& TextBox::build() {
    m_text->actualize_height();

    Button::build();
    Element::to_center(*this, *m_text);

    return (*this);
}


void TextBox::input(TextBox& text_box, const sf::RenderWindow& window, const sf::Event& event) {
    if(event.type == sf::Event::TextEntered) {
        if(!text_box.is_focused()) return;

        auto& text = text_box.m_text;
        const auto& size = text->get_size();

        auto last_pos = (size > 0) ? size - 1 : 0;

        switch (event.text.unicode) {
            case 0x8:
                if(text->is_empty()) break;

                if(text_box[last_pos] == '|') {
                    if(size < 2) break;

                    text->remove_char(last_pos - 1);
                } else {
                    text->narrow_text(0, 1);
                }

                break ;
            default: {
                if(size >= text_box.m_max_characters_number) break;

                if(event.text.unicode == 13) { // Enter
                    text_box.delete_focus();
                    break;
                }

                bool is_space = event.text.unicode == 32;
                bool is_char = event.text.unicode >= 65 && event.text.unicode <= 90;
                bool is_upper_char = event.text.unicode >= 97 && event.text.unicode <= 122;

                if(!is_char && !is_upper_char && !is_space) break;

                if(text_box[last_pos] == '|') {
                    text->add_char(event.text.unicode, Text::Before, last_pos);
                } else {
                    text->add_char(event.text.unicode);
                }

                break;
            }
        }

        text_box.button_text_to_center();
    }
}

TextBox& TextBox::set_max_chars_number(uint8_t num) {
    m_max_characters_number = num;

    return (*this);
}

// PUBLIC GET
string TextBox::get_entered_text() const {
    return m_text->get_value();
}

char32_t TextBox::operator[] (const size_t index) {
    return (*m_text)[index];
}