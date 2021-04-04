//
// Created by user on 04.04.2021.
//

#include "MousePosition.h"

MousePosition::MousePosition(): m_prev_pos(0, 0), m_curr_pos(0, 0) {
}


MousePosition::MousePosition(const sf::RenderWindow& w): m_prev_pos(sf::Mouse::getPosition(w)), m_curr_pos(sf::Mouse::getPosition(w)) {
}

// PUBLIC SET
void MousePosition::input(MousePosition& mouse_pos, const sf::RenderWindow& w, const sf::Event& event) {
    if(event.type == sf::Event::MouseMoved) {
        mouse_pos.update_pos(w);
    }
}

MousePosition& MousePosition::update_pos(const sf::RenderWindow& w) {
    m_prev_pos = m_curr_pos;
    m_curr_pos = sf::Mouse::getPosition(w);

    return (*this);
}

// PUBLIC GET
const sf::Vector2i& MousePosition::get_prev_pos() const {
    return m_prev_pos;
}

const sf::Vector2i& MousePosition::get_curr_pos() const {
    return m_curr_pos;
}
