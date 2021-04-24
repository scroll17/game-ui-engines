//
// Created by user on 17.02.2021.
//

#include "Element.h"

using namespace form::types;

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
void Element::to_center(Element& current, Element& target, const Indentation *indentation) {
    auto border_shift = current.m_border_with_position ? current.m_border_width : 0;

    auto shift_y = (current.get_height() / 2.f) + (target.get_height() / 2.f) - border_shift; // TODO: is not required: (target.get_height() / 2.f) - border_shift
    auto shift_x = (current.get_width() / 2.f) + (target.get_width() / 2.f) + border_shift;

    target
        .set_before(XY, current)
        .move(Y, shift_y)
        .move(X, shift_x);

    if(indentation != nullptr) {
        target
            .move(X, indentation->left)
            .move(X, -indentation->right);

        target
            .move(Y, indentation->top)
            .move(Y, -indentation->bottom);
    }

    target.build();
}

Element& Element::set_size(const sf::Vector2f& size) {
    this->set_width(size.x);
    this->set_height(size.y);

    return (*this);
}

Element& form::types::Element::set_width(const float& width) {
    m_bounds.width = width;

    this->turn_off_building();
    return (*this);
}

Element& Element::set_height(const float& height) {
    m_bounds.height = height;

    this->turn_off_building();
    return (*this);
}

Element& Element::set_window_size(const sf::Vector2u& size, bool correct_pos) {
    m_window_size = size;

    if(correct_pos) {
        this->set_origin_to_center(XY);
        this->correct_position(XY);
    }

    this->turn_off_building();
    return (*this);
}

Element& Element::set_border_width(const float& w) {
    auto prev_width = m_border_width;

    m_border_width = w;

    if(m_border_with_position) {
        this->calculate_position_with_border(prev_width, m_border_width);
    }

    this->turn_off_building();
    return (*this);
}

Element& Element::set_border_color(const sf::Color& color) {
    m_border_color = color;

    this->turn_off_building();
    return (*this);
}

Element& Element::border_with_position(bool solution) {
    if(m_border_with_position == solution) return (*this);

    auto prev_status = m_border_with_position;

    m_border_with_position = solution;
    if(m_border_with_position) {
        this->calculate_position_with_border(0, m_border_width);
    } else {
        if(prev_status) {
            this->calculate_position_with_border(m_border_width, 0);
        }
    }

    return (*this);
}

Element& Element::correct_position(bool solution) {
    if(m_need_correct_pos == solution) return (*this);

    m_need_correct_pos = solution;
    if(m_need_correct_pos) this->correct_position(XY);

    return (*this);
}

Element& Element::set_position(const sf::Vector2f& position) {
    this->change_position(X, position.x);
    this->change_position(Y, position.y);

    this->turn_off_building();
    return (*this);
}

Element& Element::set_after(const Element::Axis& axis, const Element& el) {
    const auto& bounds = el.get_bounds();
    const auto& origin = el.get_origin();

    auto border_shift = el.m_border_with_position ? el.m_border_width : 0;

    if(axis == X || axis == XY) {
        auto shift = bounds.left + border_shift + (m_bounds.width / 2.f);
        auto start_pos = origin.x > 0 ? origin.x : bounds.width;

        this->change_position(X, start_pos + shift);
    }

    if(axis == Y || axis == XY) {
        auto shift = bounds.top + border_shift + (m_bounds.height / 2.f);
        auto start_pos = origin.y > 0 ? origin.y : bounds.height;

        this->change_position(Y, start_pos + shift);
    }

    this->turn_off_building();
    return (*this);
}

Element& Element::set_before(const Element::Axis& axis, const Element& el) {
    const auto& bounds = el.get_bounds();
    const auto& origin = el.get_origin();

    auto border_shift = el.m_border_with_position ? el.m_border_width : 0;
    auto local_border_shift = m_border_with_position ? m_border_width * 2 : 0;

    if(axis == X || axis == XY) {
        auto shift = el.m_origin.x == 0
          ? 0
          : bounds.left - origin.x - border_shift - (m_bounds.width / 2.f) - local_border_shift;

        this->change_position(X, shift);
    }

    if(axis == Y || axis == XY) {
        auto shift = el.m_origin.y == 0
           ? 0
           : bounds.top - origin.y - border_shift - (m_bounds.height / 2.f) - local_border_shift;

        this->change_position(Y, shift);
    }

    this->turn_off_building();
    return (*this);
}

Element& Element::set_after_position(const Element::Axis& axis, const sf::Vector2f& position) {
    if(axis == X || axis == XY) {
        this->change_position(X, position.x + (m_bounds.width / 2.f));
    }

    if(axis == Y || axis == XY) {
        this->change_position(Y, position.y + (m_bounds.height / 2.f));
    }

    this->turn_off_building();
    return (*this);
}

Element& Element::set_before_position(const Element::Axis& axis, const sf::Vector2f& position) {
    auto local_border_shift = m_border_with_position ? m_border_width : 0;

    if(axis == X || axis == XY) {
        this->change_position(X, position.x - (m_bounds.width / 2.f) - (local_border_shift * 2));
    };
    if(axis == Y || axis == XY) {
        this->change_position(Y, position.y - (m_bounds.height / 2.f) - (local_border_shift * 2));
    };

    this->turn_off_building();
    return (*this);
}

Element& Element::to_center() {
    this->set_origin_to_center(XY);

    m_bounds.left = (m_window_size.x / 2.f);
    m_bounds.top = (m_window_size.y / 2.f);

    this->correct_position(XY);
    this->turn_off_building();

    return (*this);
}

Element& Element::to_center(const Element::Axis& axis) {
    if(axis == XY) return Element::to_center();

    if(axis == X) this->change_position(axis, m_window_size.x / 2.f);
    if(axis == Y) this->change_position(axis, m_window_size.y / 2.f);

    this->turn_off_building();
    return (*this);
}

Element& Element::to(const Element::Angle& angel) {
    switch (angel) {
        case Top: {
            this->change_position(Y, m_bounds.height / 2.f);
            break;
        }
        case Down: {
            this->change_position(Y, m_window_size.y - (m_bounds.height / 2.f));
            break;
        }
        case Left: {
            this->change_position(X, m_bounds.width / 2.f);
            break;
        }
        case Right: {
            this->change_position(X, m_window_size.x - (m_bounds.width / 2.f));
            break;
        }
    }

    this->turn_off_building();
    return (*this);
}

Element& Element::move(const Element::Axis& axis, const float& pixels) {
    if(axis == XY) {
        throw std::runtime_error("XY is invalid argument");
    }

    if(axis == X) {
        auto shift = pixels + (m_origin.x > 0 ? 0 : m_bounds.width / 2.f);
        this->change_position(axis, m_bounds.left + shift);
    };
    if(axis == Y) {
        auto shift = pixels + (m_origin.y > 0 ? 0 : m_bounds.height / 2.f);
        this->change_position(axis, m_bounds.top + shift);
    };

    this->turn_off_building();
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
    m_build_count++;
}

void Element::turn_off_building() {
    m_need_build = true;
}

void Element::calculate_position_with_border(const float& prev_width, const float& next_width) {
    m_bounds.left -= prev_width;
    m_bounds.top -= prev_width;

    m_bounds.left += next_width;
    m_bounds.top += next_width;

    this->correct_position(XY);
    this->turn_off_building();
}

void Element::change_position(const Element::Axis& axis, const float& pixels) {
    if(axis == XY) {
        throw std::runtime_error("XY is invalid argument");
    }

    this->set_origin_to_center(axis);

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

    this->correct_position(axis);
    this->turn_off_building();
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

    this->turn_off_building();
}

void Element::set_origin_to_center(const Element::Axis& axis, const sf::FloatRect *el_rect) {
    const sf::FloatRect& rect = el_rect == nullptr ? this->get_bounds() : (*el_rect);

    if(axis == X || axis == XY) m_origin.x = rect.width / 2.f;
    if(axis == Y || axis == XY) m_origin.y = rect.height / 2.f;

    this->turn_off_building();
}