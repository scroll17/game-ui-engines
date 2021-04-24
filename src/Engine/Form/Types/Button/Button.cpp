//
// Created by user on 09.02.2021.
//

#include "Button.h"

using namespace form::types;

Button::Button(const sf::Vector2f& size, const string& str): ActionElement(size) {
    m_border_color = sf::Color::Black;

    m_rectangle = new sf::RectangleShape();
    m_text = new Text(str);
}

Button::Button(const sf::Vector2f& size, const sf::Vector2f& pos, const string& str): ActionElement(size, pos) {
    m_border_color = sf::Color::Black;

    m_rectangle = new sf::RectangleShape();
    m_text = new Text(str);
}

Button::~Button() {
    delete m_rectangle;
    delete m_text;
}

// PUBLIC VOID
void Button::draw(sf::RenderWindow& window) const {
    if(m_need_build) throw data_types::Exception(data_types::Exception::ElementNotBuild);

    window.draw(*m_rectangle);
    m_text->draw(window);
}

void Button::input(Button& button, const sf::RenderWindow& window, const sf::Event& event, const sf::Vector2i& prev_pos) {
    if(event.type == sf::Event::MouseButtonPressed) {
        Button::handle_mouse_button_pressed_e(button, window);
    }

    if(event.type == sf::Event::MouseButtonReleased) {
        Button::handle_mouse_button_released_e(button, window);
    }

    if(event.type == sf::Event::MouseMoved) {
        Button::handle_mouse_moved_e(button, window, prev_pos);
    }
}

// PUBLIC GET
Text& Button::get_button_text() const {
    return (*m_text);
}

// PUBLIC SET
Button& Button::build() {
    /// TEXT
    if(m_text->m_build_count == 0) {
        m_text->correct_position(true);
    }

    m_text->build();

    /// RECTANGLE SHAPE
    if(!m_need_build) return (*this);

    m_rectangle->setSize(this->get_size());
    m_rectangle->setFillColor(m_bg_color);

    m_rectangle->setOutlineThickness(m_border_width);
    m_rectangle->setOutlineColor(m_border_color);

    m_rectangle->setPosition(this->get_position());
    m_rectangle->setOrigin(this->get_origin());

    this->turn_on_building();

    return (*this);
}

Button& Button::set_window_size(const sf::Vector2u& size, bool correct_pos) {
    m_text->set_window_size(size);

    Element::set_window_size(size, correct_pos);

    return (*this);
}

Button& Button::button_text_to_center() {
    if(m_rectangle == nullptr || m_text == nullptr) {
        throw std::runtime_error("Required vars no inited");
    }

    m_text->actualize_size();
    Element::to_center(*this, *m_text);

    return (*this);
}

Button& Button::set_text(const string& text) {
    m_text->set_text(text);
    return (*this);
}

Button& Button::set_text_size(int size) {
    m_text->set_text_size(size);
    return (*this);
}

Button& Button::add_text(const string& text) {
    m_text->add_text(text);
    return (*this);
}

Button& Button::narrow_text(size_t start_count, size_t end_count) {
    m_text->narrow_text(start_count, end_count);

    return (*this);
}

Button& Button::set_text_color(const sf::Color& color) {
    m_text->set_color(color);
    return (*this);
}

Button& Button::set_bg_color(const sf::Color& color) {
    m_bg_color = color;

    this->turn_off_building();
    return (*this);
}

// PROTECTED SET
void Button::handle_mouse_button_pressed_e(Button& button, const sf::RenderWindow& window) {
    bool on_button = Form::mouse_in(button, window);

    if(on_button) {
        button.click();
    }
}

void Button::handle_mouse_button_released_e(Button& button, const sf::RenderWindow& window) {
    bool on_button = Form::mouse_in(button, window);

    if(on_button) {
        button.after_click();
    }
}

void Button::handle_mouse_moved_e(Button& button, const sf::RenderWindow& window, const sf::Vector2i& prev_pos) {
    bool prev_pos_on_button = Form::mouse_in(button, prev_pos);
    bool curr_pos_on_button = Form::mouse_in(button, window);

    if(curr_pos_on_button && !prev_pos_on_button) {
        button.hover();
    }

    if(prev_pos_on_button && !curr_pos_on_button) {
        button.after_hover();
    }
}
