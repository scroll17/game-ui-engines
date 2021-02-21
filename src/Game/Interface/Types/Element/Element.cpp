//
// Created by user on 17.02.2021.
//

#include "Element.h"

Element::Element(const sf::Vector2f& size) {
    m_bounds.width = size.x;
    m_bounds.height = size.y;
}

Element::Element(const sf::Vector2f& size, const sf::Vector2f& position) {
    m_bounds.width = size.x;
    m_bounds.height = size.y;

    m_bounds.left = position.x;
    m_bounds.top = position.y;
}

// PUBLIC SET
Element& Element::set_size(const sf::Vector2f& size) {
    set_width(size.x);
    set_height(size.y);

    return (*this);
}

Element& Element::set_width(const float& width) {
    m_bounds.width = width;

    turn_off_building();
    return (*this);
}

Element& Element::set_height(const float& height) {
    m_bounds.height = height;

    turn_off_building();
    return (*this);
}

Element& Element::set_window_size(const sf::Vector2u& size, bool correct_pos) {
    m_window_size = size;

    if(correct_pos) {
        set_origin_to_center(XY);
        correct_position(XY);
    }

    turn_off_building();
    return (*this);
}

Element& Element::set_border_width(const float& w) {
    auto prev_width = m_border_width;

    m_border_width = w;

    if(m_border_with_position) {
        calculate_position_with_border(prev_width, m_border_width);
    }

    turn_off_building();
    return (*this);
}

Element& Element::set_border_color(const sf::Color& color) {
    m_border_color = color;

    turn_off_building();
    return (*this);
}

Element& Element::border_with_position(bool solution) {
    if(m_border_with_position == solution) return (*this);

    auto prev_status = m_border_with_position;

    m_border_with_position = solution;
    if(m_border_with_position) {
        calculate_position_with_border(0, m_border_width);
    } else {
        if(prev_status) {
            calculate_position_with_border(m_border_width, 0);
        }
    }

    return (*this);
}

Element& Element::correct_position(bool solution) {
    if(m_need_correct_pos == solution) return (*this);

    m_need_correct_pos = solution;
    if(m_need_correct_pos) correct_position(XY);

    return (*this);
}

Element& Element::set_position(const sf::Vector2f& position) {
    change_position(X, position.x);
    change_position(Y, position.y);

    turn_off_building();
    return (*this);
}

Element& Element::set_after(const Element::Axis& axis, const Element& el) {
    auto bounds = el.get_bounds();
    auto border_shift = el.m_border_with_position ? el.m_border_width : 0;

    if(axis == X || axis == XY) {
        auto shift = bounds.left + border_shift + (m_bounds.width / 2.f);
        auto start_pos = el.m_origin.x > 0 ? el.m_origin.x : bounds.width;

        change_position(X, start_pos + shift);
    }

    if(axis == Y || axis == XY) {
        auto shift = bounds.top + border_shift + (m_bounds.height / 2.f);
        auto start_pos = el.m_origin.y > 0 ? el.m_origin.y : bounds.height;

        change_position(Y, start_pos + shift);
    }

    turn_off_building();
    return (*this);
}

Element& Element::set_before(const Element::Axis& axis, const Element& el) {
    auto bounds = el.get_bounds();

    auto border_shift = el.m_border_with_position ? el.m_border_width : 0;
    auto local_border_shift = m_border_with_position ? m_border_width : 0;

    if(axis == X || axis == XY) {
        auto shift = el.m_origin.x == 0
          ? 0
          : bounds.left - el.m_origin.x - border_shift - (m_bounds.width / 2.f) - (local_border_shift * 2);

        change_position(X, shift);
    }

    if(axis == Y || axis == XY) {
        auto shift = el.m_origin.y == 0
           ? 0
           : bounds.top - el.m_origin.y - border_shift - (m_bounds.height / 2.f) - (local_border_shift * 2);

        change_position(Y, shift);
    }

    turn_off_building();
    return (*this);
}

Element& Element::set_after_position(const Element::Axis& axis, const sf::Vector2f& position) {
    if(axis == X || axis == XY) change_position(X, position.x + (m_bounds.width / 2.f));
    if(axis == Y || axis == XY) change_position(Y, position.y + (m_bounds.height / 2.f));

    turn_off_building();
    return (*this);
}

Element& Element::set_before_position(const Element::Axis& axis, const sf::Vector2f& position) {
    auto local_border_shift = m_border_with_position ? m_border_width : 0;

    if(axis == X || axis == XY) {
        change_position(X, position.x - (m_bounds.width / 2.f) - (local_border_shift * 2));
    };
    if(axis == Y || axis == XY) {
        change_position(Y, position.y - (m_bounds.height / 2.f) - (local_border_shift * 2));
    };

    turn_off_building();
    return (*this);
}

Element& Element::to_center() {
    set_origin_to_center(XY);

    m_bounds.left = (m_window_size.x / 2.f);
    m_bounds.top = (m_window_size.y / 2.f);

    correct_position(XY);

    turn_off_building();
    return (*this);
}

Element& Element::to_center(const Element::Axis& axis) {
    if(axis == XY) return Element::to_center();

    if(axis == X) change_position(axis, m_window_size.x / 2.f);
    if(axis == Y) change_position(axis, m_window_size.y / 2.f);

    turn_off_building();
    return (*this);
}

Element& Element::to(const Element::Angle& angel) {
    switch (angel) {
        case Top: {
            change_position(Y, m_bounds.height / 2.f);
            break;
        }
        case Down: {
            change_position(Y, m_window_size.y - (m_bounds.height / 2.f));
            break;
        }
        case Left: {
            change_position(X, m_bounds.width / 2.f);
            break;
        }
        case Right: {
            change_position(X, m_window_size.x - (m_bounds.width / 2.f));
            break;
        }
    }

    turn_off_building();
    return (*this);
}

Element& Element::move(const Element::Axis& axis, const float& pixels) {
    if(axis == XY) {
        throw std::runtime_error("XY is invalid argument");
    }

    if(axis == X) {
        auto shift = pixels + (m_origin.x > 0 ? 0 : m_bounds.width / 2.f);
        change_position(axis, m_bounds.left + shift);
    };
    if(axis == Y) {
        auto shift = pixels + (m_origin.y > 0 ? 0 : m_bounds.height / 2.f);
        change_position(axis, m_bounds.top + shift);
    };

    turn_off_building();
    return (*this);
}

// PUBLIC GET
const sf::FloatRect& Element::get_bounds() const {
    return m_bounds;
}

const sf::Vector2f& Element::get_origin() const {
    return m_origin;
}

sf::Vector2f Element::get_size() const {
    return sf::Vector2f(m_bounds.width, m_bounds.height);
}

sf::Vector2f Element::get_position() const {
    return sf::Vector2f(m_bounds.left, m_bounds.top);
}

const float& Element::get_x() const {
    return m_bounds.left;
}

const float& Element::get_y() const {
    return m_bounds.top;
}

const float& Element::get_width() const {
    return m_bounds.width;
}

const float& Element::get_height() const {
    return m_bounds.height;
}

// PROTECTED SET
void Element::turn_on_building() {
    m_need_build = false;
}

void Element::turn_off_building() {
    m_need_build = true;
}

void Element::calculate_position_with_border(const float& prev_width, const float& next_width) {
    m_bounds.left -= prev_width;
    m_bounds.top -= prev_width;

    m_bounds.left += next_width;
    m_bounds.top += next_width;

    correct_position(XY);

    turn_off_building();
}

void Element::change_position(const Element::Axis& axis, const float& pixels) {
    if(axis == XY) {
        throw std::runtime_error("XY is invalid argument");
    }

    set_origin_to_center(axis);

    if(axis == X) {
        m_bounds.left = pixels;

        if(m_border_with_position) {
            m_bounds.left += m_border_width;
        }
    };
    if(axis == Y) {
        m_bounds.top = pixels;

        if(m_border_with_position) {
            m_bounds.top += m_border_width;
        }
    }

    correct_position(axis);

    turn_off_building();
}

void Element::correct_position(const Element::Axis& axis) {
    if(!m_need_correct_pos) return;

    auto border_shift = m_border_with_position ? m_border_width : 0;

    if(axis == X || axis == XY) {
        if (m_bounds.left <= m_origin.x) {
            m_bounds.left = m_origin.x + border_shift;
        }

        auto going_beyond = m_window_size.x - (m_bounds.left + m_origin.x) - border_shift;
        if(going_beyond < 0) {
            m_bounds.left += going_beyond;
        }
    }

    if(axis == Y || axis == XY) {
        if(m_bounds.top <= m_origin.y) {
            m_bounds.top = m_origin.y + border_shift;
        }

        auto going_beyond = m_window_size.y - (m_bounds.top + m_origin.y) - border_shift;
        if(going_beyond < 0) {
            m_bounds.top += going_beyond;
        }
    }

    turn_off_building();
}

void Element::set_origin_to_center(const Element::Axis& axis, const sf::FloatRect *el_rect) {
    const sf::FloatRect& rect = el_rect == nullptr ? get_bounds() : (*el_rect);

    if(axis == X || axis == XY) m_origin.x = rect.width / 2.f;
    if(axis == Y || axis == XY) m_origin.y = rect.height / 2.f;

    turn_off_building();
}