//
// Created by user on 09.02.2021.
//

#include "Button.h"


Button::Button(const sf::Vector2f& size, const string& str): ActionElement<Button>(size), m_str(str) {
    m_border_color = sf::Color::Black;

    m_rectangle = new sf::RectangleShape();
    m_text = new sf::Text();
}

Button::Button(const sf::Vector2f& size, const sf::Vector2f& pos, const string& str): ActionElement<Button>(size, pos), m_str(str) {
    m_border_color = sf::Color::Black;

    m_rectangle = new sf::RectangleShape();
    m_text = new sf::Text();
}

Button::~Button() {
    delete m_rectangle;
    delete m_text;
}

// PUBLIC VOID
void Button::draw(sf::RenderWindow& window) const {
    if(m_need_build) throw Exception(Exception::ElementNotBuild);

    window.draw(*m_rectangle);
    window.draw(*m_text);
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

// PUBLIC SET
Button& Button::build() {
    if(!m_need_build) return (*this);

    /// RECTANGLE SHAPE
    m_rectangle->setSize(this->get_size());
    m_rectangle->setFillColor(m_bg_color);

    m_rectangle->setOutlineThickness(m_border_width);
    m_rectangle->setOutlineColor(m_border_color);

    m_rectangle->setPosition(this->get_position());
    m_rectangle->setOrigin(this->get_origin());

    /// TEXT
    m_text->setFont(Engine::get_game_font());
    m_text->setString(sf::String::fromUtf8(m_str.begin(), m_str.end()));

    m_text->setCharacterSize(m_text_size);
    m_text->setFillColor(m_text_color);

    this->button_text_to_center();
    this->turn_on_building();

    return (*this);
}

Button& Button::set_text(const string& text) {
    m_str = text;

    this->turn_off_building();
    return (*this);
}

Button& Button::set_text_size(int size) {
    m_text_size = size;

    this->turn_off_building();
    return (*this);
}

Button& Button::set_text_color(const sf::Color& color) {
    m_text_color = color;

    this->turn_off_building();
    return (*this);
}

Button& Button::set_bg_color(const sf::Color& color) {
    m_bg_color = color;

    this->turn_off_building();
    return (*this);
}

// PROTECTED SET
void Button::button_text_to_center() {
    if(m_rectangle == nullptr || m_text == nullptr) {
        throw std::runtime_error("Required vars no inited");
    }

    const auto& text_locals = m_text->getLocalBounds();
    m_text->setOrigin({
        (text_locals.width / 2.f) + text_locals.left,
        (text_locals.height / 2.f) + text_locals.top
    });

    const auto& locals = m_rectangle->getGlobalBounds();
    m_text->setPosition({
        (locals.width / 2.f) + locals.left,
        (locals.height / 2.f) + locals.top
    });
}

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
