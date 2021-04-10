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

// PUBLIC GET
string Text::get_value() const {
    return m_value->getString();
}

size_t Text::get_size() const {
    return m_value->getString().getSize();
}

bool Text::is_empty() const {
    return m_value->getString().isEmpty();
}

char32_t Text::operator[](const size_t index) {
    return m_value->getString()[index];
}

// PUBLIC SET
Text& Text::build() {
    if(!m_need_build) return (*this);

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

Text& Text::add_char(char ch, const ActionPosition& action, size_t pos) {
    auto curr_str = string(m_value->getString());

    switch (action) {
        case Current: {
            curr_str.append(&ch);
            break;
        }
        case Before: {
            curr_str.insert(pos, &ch);
            break;
        }
        case After: {
            curr_str.insert(pos + 1, &ch);
            break;
        }
    }

    m_value->setString(curr_str);

    return (*this);
}

Text& Text::add_text(const string& str, const Text::ActionPosition& action, size_t pos) {
    auto curr_str = string(m_value->getString());

    switch (action) {
        case Current: {
            curr_str.append(str);
            break;
        }
        case Before: {
            curr_str.insert(pos, str);
            break;
        }
        case After: {
            curr_str.insert(pos + 1, str);
            break;
        }
    }

    m_value->setString(curr_str);

    return (*this);
}

Text& Text::remove_char(size_t pos) {
    auto curr_str = string(m_value->getString());
    curr_str.erase(pos, 1);

    m_value->setString(curr_str);

    return (*this);
}

Text& Text::remove_chars(size_t pos, size_t count) {
    auto curr_str = string(m_value->getString());

    auto start = curr_str.at(pos);
    auto end = start + count;

    curr_str.erase(start, end);

    m_value->setString(curr_str);

    return (*this);
}

Text& Text::narrow_text(size_t start_count, size_t end_count) {
    const auto& str = m_value->getString();
    const auto& size = str.getSize();

    if((start_count + end_count) > size) throw std::runtime_error("invalid argument");
    if(start_count > size || end_count > size) throw std::runtime_error("invalid argument");

    auto begin(str.begin()), end(str.end());

    if(start_count != 0) begin += start_count;
    if(end_count != 0) end -= end_count;

    m_value->setString(sf::String::fromUtf8(begin, end));

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