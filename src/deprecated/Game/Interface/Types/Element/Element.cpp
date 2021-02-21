//
// Created by user on 09.02.2021.
//

#include "Element.h"

// PROTECTED SET
/** @deprecated */
void Element::turn_off_building() {
    m_builded = false;
}

/** @deprecated */
void Element::turn_on_building() {
    m_builded = true;
}

void Element::turn_on_boundary(int coordinate = -1) {
    if(coordinate == 0) {
        m_actual_x_bounds = true;
    } else if(coordinate == 1) {
        m_actual_y_bounds = true;
    } else if(coordinate == -1) {
        m_actual_x_bounds = m_actual_y_bounds = true;
    } else {
        throw std::runtime_error("Invalid argument");
    }
}

void Element::turn_off_boundary(int coordinate = -1) {
    if(coordinate == 0) {
        m_actual_x_bounds = false;
    } else if(coordinate == 1) {
        m_actual_y_bounds = false;
    } else if(coordinate == -1) {
        m_actual_x_bounds = m_actual_y_bounds = false;
    } else {
        throw std::runtime_error("Invalid argument");
    }
}

/** @deprecated */
void Element::calculate_position_with_border(float prev_with, float next_with) {
    if(prev_with != 0) {
        calculate_bounds();
        set_correct_origin();
    }

    // TODO _ refactor

    m_position.x -= prev_with;
    m_position.y -= prev_with;

    if(prev_with != 0) {
        calculate_bounds();
        set_correct_origin();
    }

    calculate_bounds();
    set_correct_origin();

    m_position.x += next_with;
    m_position.y += next_with;

    turn_off_building();
}

/** @deprecated */
void Element::calculate_bounds() {
    if(m_actual_x_bounds && m_actual_y_bounds) return;

    calculate_bounds_x();
    calculate_bounds_y();
}

/** @deprecated */
void Element::calculate_bounds_x() {
    if(m_actual_x_bounds) return;

    m_bounds.width = m_size.x;
    m_bounds.left = m_position.x;

    turn_on_boundary(0);
}

/** @deprecated */
void Element::calculate_bounds_y() {
    if(m_actual_y_bounds) return;

    m_bounds.height = m_size.y;
    m_bounds.top = m_position.y;

    turn_on_boundary(1);
}

/** @deprecated */
void Element::set_correct_origin(bool use_x, bool use_y, const sf::FloatRect *el_rect) {
    const sf::FloatRect& rect = el_rect == nullptr ? get_bounds() : (*el_rect);

    if(use_x) {
        auto pos_x = rect.left + (rect.width / 2.f);
        auto x_shift = m_position.x - pos_x;

        if(x_shift < 0) pos_x += x_shift;

        m_origin.x = pos_x;
        turn_off_building();
    }

    if(use_y) {
        auto pos_y = rect.top + (rect.height / 2.f);
        auto y_shift = m_position.y - pos_y;

        if(y_shift < 0) pos_y += y_shift;

        m_origin.y = pos_y;
        turn_off_building();
    }
}

/** @deprecated */
void Element::origin_to_center() {
    Element::origin_to_center(get_bounds());
}

/** @deprecated */
void Element::origin_to_center(const sf::FloatRect& el_rect) {
    set_correct_origin(true, true, &el_rect);
//    origin_x_to_center(el_rect);
//    origin_y_to_center(el_rect);

    turn_off_building();
}

/** @deprecated */
void Element::origin_x_to_center(const float& left, const float& width) {
    m_origin.x = left + (width / 2.f);
}

/** @deprecated */
void Element::origin_y_to_center(const float& top, const float& height) {
    m_origin.y = top + (height / 2.f);
}

/** @deprecated */
void Element::origin_x_to_center(const sf::FloatRect& el_rect) {
    Element::origin_x_to_center(el_rect.left, el_rect.width);
}

/** @deprecated */
void Element::origin_y_to_center(const sf::FloatRect& el_rect) {
    Element::origin_y_to_center(el_rect.top, el_rect.height);
}


// PUBLIC SET
Element& Element::set_size(const sf::Vector2f& size) {
    m_size = size;

    turn_off_boundary();
    turn_off_building();

    return (*this);
}

using namespace std;

/** @deprecated */
Element& Element::set_border_width(const float w) {
    cout << "--- BEFORE " << endl;
    cout << "m_position.x = " << m_position.x << endl;
    cout << "m_position.y = " << m_position.y << endl;
    cout << "m_origin.x = " << m_origin.x << endl;
    cout << "m_origin.y = " << m_origin.y << endl;

    if(m_position_with_border) {
        calculate_position_with_border(m_border_width, w);
    }

    m_border_width = w;

    cout << "--- AFTER " << endl;
    cout << "m_position.x = " << m_position.x << endl;
    cout << "m_position.y = " << m_position.y << endl;
    cout << "m_origin.x = " << m_origin.x << endl;
    cout << "m_origin.y = " << m_origin.y << endl;

    turn_off_building();
    return (*this);
}

/** @deprecated */
Element& Element::set_border_color(const sf::Color& color) {
    m_border_color = color;

    turn_off_building();
    return (*this);
}

/** @deprecated */
Element &Element::calculate_position_with_border(bool solution) {
    if(m_position_with_border && !solution) {
        // TODO _
        // ... remove borders from position
    }

    m_position_with_border = solution;

    if(m_position_with_border) {
        // TODO _
        // ... add border to position
    }

    return (*this);
}

/** @deprecated */
Element& Element::set_position(const sf::Vector2f& position) {
    calculate_bounds();
    origin_to_center();

    m_position = position;

    turn_off_building();
    return (*this);
}

Element& Element::set_after_y_position(const Element& el) {
    calculate_bounds_y();
    origin_y_to_center(get_bounds());

    auto bounds = el.get_bounds();
    m_position.y = (bounds.top + bounds.height) + m_border_width;

    turn_off_building();
    return (*this);
}

Element& Element::set_after_x_position(const Element& el) {
    calculate_bounds_x();
    origin_x_to_center(get_bounds());

    auto bounds = el.get_bounds();
    m_position.x = (bounds.left + bounds.width) + m_border_width;

    turn_off_building();
    return (*this);
}

Element& Element::set_before_y_position(const Element& el) {
    calculate_bounds_y();
    origin_y_to_center(get_bounds());

    auto bounds = el.get_bounds();
    m_position.y = bounds.top - (m_border_width - m_size.y);

    turn_off_building();
    return (*this);
}

Element& Element::set_before_x_position(const Element& el) {
    calculate_bounds_x();
    origin_x_to_center(get_bounds());

    auto bounds = el.get_bounds();
    m_position.x = bounds.left - (m_border_width - m_size.x);

    turn_off_building();
    return (*this);
}

/** @deprecated */
Element& Element::to_center(const sf::Vector2u& window_size) {
    to_center_x(window_size);
    to_center_y(window_size);

    return (*this);
}

Element& Element::to_center_x(const sf::Vector2u& window_size) {
    calculate_bounds_x();

    m_position.x = (window_size.x / 2.f);

    set_correct_origin(true, false);

    turn_off_building();
    return (*this);
}

Element& Element::to_center_y(const sf::Vector2u& window_size) {
    calculate_bounds_y();

    m_position.y = (window_size.y / 2.f);

    set_correct_origin(false, true);

    turn_off_building();
    return (*this);
}

Element& Element::to_right_top(const sf::Vector2u& window_size) {
    calculate_bounds_x();

    m_position.x = window_size.x - ((m_size.x / 2.f) + m_border_width);

    set_correct_origin(true, false);

    turn_off_building();
    return (*this);
}

Element& Element::to_right_down(const sf::Vector2u &window_size) {
    calculate_bounds();

    m_position.x = window_size.x - ((m_size.x / 2.f) + m_border_width);
    m_position.y = window_size.y - ((m_size.y / 2.f) + m_border_width);

    set_correct_origin();

    turn_off_building();
    return (*this);
}

Element& Element::to_left_top(const sf::Vector2u &window_size) {
    std::cout << "--- AFTER " << std::endl;
    std::cout << "m_position.x = " << m_position.x << std::endl;
    std::cout << "m_position.y = " << m_position.y << std::endl;

    if(m_position.x != 0) {
        calculate_bounds_x();
        origin_x_to_center(get_bounds());

        m_position.x = 450; //250 + (m_size.x / 2.f) - m_border_width;

        turn_off_building();
    }

    if(m_position.y != 0) {
        calculate_bounds_y();
        origin_y_to_center(get_bounds());

        m_position.y = 175; //(m_size.y / 2.f) - m_border_width;

        turn_off_building();
    }

//    calculate_bounds();
//    origin_to_center();


//    m_position.x = 150;//(m_size.x / 2.f) - m_border_width;
//    m_position.y = 175;//(m_size.y / 2.f) - m_border_width;

    std::cout << "--- BEFORE " << std::endl;
    std::cout << "m_position.x = " << m_position.x << std::endl;
    std::cout << "m_position.y = " << m_position.y << std::endl;


    return (*this);
}

Element& Element::to_left_down(const sf::Vector2u& window_size) {
    return (*this);
}

/** @deprecated */
Element& Element::moving_by_x(float val) {
    calculate_bounds_x();
    origin_x_to_center(get_bounds());

    m_position.x += val;

    turn_off_building();
    return (*this);
}

/** @deprecated */
Element& Element::moving_by_y(float val) {
    calculate_bounds_y();
    origin_y_to_center(get_bounds());

    m_position.y += val;

    turn_off_building();
    return (*this);
}


// PUBLIC GET
const sf::Vector2f& Element::get_position() const {
    return m_position;
}

const sf::Vector2f& Element::get_origin() const {
    return m_origin;
}

const sf::Vector2f& Element::get_size() const {
    return m_size;
}

float Element::get_height() const {
    return m_size.y;
}

float Element::get_width() const {
    return m_size.x;
}

sf::FloatRect Element::get_bounds() const {
    return m_bounds;
}

//const sf::FloatRect& Element::get_bounds() const {
//    return m_bounds;
//}
