//
// Created by user on 16.02.2021.
//

#include "Interface.h"

bool FormInterface::mouse_in(const Element& el, const sf::RenderWindow& window) {
    return FormInterface::mouse_in(el, sf::Mouse::getPosition(window));
}

bool FormInterface::mouse_in(const Element& el, const sf::Vector2i& mouse_pos) {
    auto bounds = el.get_bounds();
    auto origin = el.get_origin();

    bool x_in = (mouse_pos.x >= (bounds.left - origin.x)) && (mouse_pos.x <= (bounds.left + origin.x));
    bool y_in = (mouse_pos.y >= (bounds.top - origin.y)) && (mouse_pos.y <= (bounds.top + origin.y));

    return x_in && y_in;
}

float FormInterface::get_center(const sf::RenderWindow& window) {
    return window.getSize().x / 2.f;
}
