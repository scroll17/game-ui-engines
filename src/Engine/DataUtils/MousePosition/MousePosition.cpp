//
// Created by user on 04.04.2021.
//

#include "MousePosition.h"

MousePosition::t_instances MousePosition::s_m_instances = {};

MousePosition::MousePosition(): m_prev_pos(0, 0), m_curr_pos(0, 0) {
}

MousePosition::MousePosition(const sf::RenderWindow& w): m_prev_pos(sf::Mouse::getPosition(w)), m_curr_pos(sf::Mouse::getPosition(w)) {
}

// PUBLIC SET
MousePosition& MousePosition::update_pos(const sf::RenderWindow& w) {
    m_prev_pos = m_curr_pos;
    m_curr_pos = sf::Mouse::getPosition(w);

    return (*this);
}

void MousePosition::input(MousePosition& mouse_pos, const sf::RenderWindow& w, const sf::Event& event) {
    if(event.type == sf::Event::MouseMoved) {
        mouse_pos.update_pos(w);
    }
}

MousePosition& MousePosition::get_instance(const sf::RenderWindow *window) {
    auto it = s_m_instances.find(window);
    if(it != std::end(s_m_instances)) {
        return *(it->second);
    } else {
        return *(s_m_instances[window] = new MousePosition());
    }
}

bool MousePosition::remove_instance(const sf::RenderWindow *window) {
    auto it = s_m_instances.find(window);
    if(it != std::end(s_m_instances)) {
        delete it->second;
        s_m_instances.erase(it);

        return true;
    }

    return false;
}

bool MousePosition::remove_instance(const MousePosition *m_p) {
    auto it = std::find_if(
            std::begin(s_m_instances),
            std::end(s_m_instances),
            [m_p](const MousePosition::t_instance& el) {
                return el.second == m_p;
            }
    );

    if(it != std::end(s_m_instances)) {
        delete it->second;
        s_m_instances.erase(it);

        return true;
    }

    return false;
}

void MousePosition::clear_all() {
    for(auto& el: s_m_instances) {
        delete el.second;
    }

    s_m_instances.clear();
}

// PUBLIC GET
const sf::Vector2i& MousePosition::get_prev_pos() const {
    return m_prev_pos;
}

const sf::Vector2i& MousePosition::get_curr_pos() const {
    return m_curr_pos;
}