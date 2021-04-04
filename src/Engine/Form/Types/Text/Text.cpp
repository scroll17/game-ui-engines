//
// Created by user on 13.02.2021.
//

#include "Text.h"

Text::Text(const string& value): Element({ 0, 0 }, { 0, 0 }) {
    this->init(value);
}

Text::Text(const string& value, sf::Font& font, sf::Color& color): Element({ 0, 0 }, { 0, 0 }), m_font(font), m_color(color) {
    this->init(value);
}

Text::~Text() {
    delete m_value;
}

// PUBLIC VOID
void Text::draw(sf::RenderWindow& window) const {
    if(m_need_build) throw Exception(Exception::ElementNotBuild);

    window.draw(*m_value);
}

// PUBLIC SET
Text& Text::build() {
    if(!m_need_build) return (*this);;

    m_value->setFont(m_font);
    m_value->setFillColor(m_color);

    m_value->setCharacterSize(m_text_size);

    m_value->setOutlineColor(m_border_color);
    m_value->setOutlineThickness(m_border_width);

    m_value->setPosition(this->get_position());
    m_value->setOrigin(this->get_origin());

    this->actualize_size();
    this->turn_on_building();

    return (*this);
}

Text& Text::set_text(const string& str) {
    m_value->setString(sf::String::fromUtf8(str.begin(), str.end()));

    this->actualize_size();
    return (*this);
}

Text& Text::set_text_size(int size) {
    m_text_size = size;

    this->turn_off_building();
    return (*this);
}

Text& Text::set_font(const sf::Font& font) {
    m_font = font;

    this->turn_off_building();
    return (*this);
}

Text& Text::set_color(const sf::Color& color) {
    m_color = color;

    this->turn_off_building();
    return (*this);
}

// PROTECTED SET
void Text::init(const string& str) {
    if(m_value != nullptr) {
        throw std::runtime_error("Already init");
    }

    m_value = new sf::Text;
    m_value->setString(sf::String::fromUtf8(str.begin(), str.end()));

    this->build();
}

void Text::actualize_size() {
    const auto& bounds = m_value->getLocalBounds();
    this->set_size({ bounds.width, bounds.height });
}
