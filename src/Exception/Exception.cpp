//
// Created by user on 11.12.2020.
//

#include "Exception.h"

#include <utility>

static map<Exception::Type, string> EXCEPTION_DETAILS {
  { Exception::Type::FileRead, "Error with read file." },
  { Exception::Type::FileNotExist, "File not found." }
};

Exception::Exception(Exception::Type type, string message): m_error_type(type), m_custom_error_message(std::move(message)) {}

const Exception::Type& Exception::get_error_type() const {
    return m_error_type;
}

const string& Exception::get_error_message() const {
    if(m_custom_error_message.length() > 0) {
        return m_custom_error_message;
    } else {
        return EXCEPTION_DETAILS[m_error_type];
    }
}

void Exception::draw_error() const {
    const int WIDTH = 450, HEIGHT = 160;
    const string error_message(get_error_message());

    sf::Text text;

    text.setFont(Game::get_game_font());
    text.setString(sf::String::fromUtf8(error_message.begin(), error_message.end()));
    text.setCharacterSize(32);
    text.setFillColor(sf::Color::White);

    sf::FloatRect text_rect = text.getLocalBounds();
    sf::Vector2f text_origin(text_rect.left + text_rect.width / 2.0f, text_rect.top  + text_rect.height / 2.0f);
    sf::Vector2f text_position(WIDTH / 2.0f,HEIGHT / 2.0f);

    text.setOrigin(text_origin);
    text.setPosition(text_position);

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Error");
    // TODO _render window on center position relative parent window

    sf::Event event {};
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        window.draw(text);
        window.display();
    }
}

const char* Exception::what() const noexcept {
    return get_error_message().c_str();
}
