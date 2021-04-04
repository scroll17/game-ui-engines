//
// Created by user on 04.04.2021.
//

#include "MousePosition.h"

MousePosition::MousePosition(const sf::RenderWindow& w): m_prev_pos(sf::Mouse::getPosition(w)), m_curr_pos(sf::Mouse::getPosition(w)) {

}

// PUBLIC SET
MousePosition& MousePosition::input(const sf::RenderWindow& w, const sf::Event& event) {
    if(event.type == sf::Event::MouseMoved) {
        m_prev_pos = m_curr_pos;
        m_curr_pos = sf::Mouse::getPosition(w);
    }

    return (*this);
}

// PUBLIC GET
const sf::Vector2i& MousePosition::get_prev_pos() const {
    return m_prev_pos;
}

const sf::Vector2i& MousePosition::get_curr_pos() const {
    return m_curr_pos;
}
