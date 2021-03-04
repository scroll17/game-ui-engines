//
// Created by user on 09.02.2021.
//

#include "ErrorForm.h"

ErrorForm::ErrorForm(string message): m_message(std::move(message)) {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    this->m_window = new sf::RenderWindow(
      sf::VideoMode(450, 150),
      "Error",
      sf::Style::Default,
      settings
    );
}

ErrorForm::ErrorForm(string message, const sf::ContextSettings& settings): m_message(std::move(message)) {
    this->m_window = new sf::RenderWindow(
      sf::VideoMode(450, 150),
      "Error",
      sf::Style::Default,
      settings
    );
}

ErrorForm::~ErrorForm() {
    delete m_window;
    delete m_text;
    delete m_button;
}

// PROTECTED SET
void ErrorForm::build() {
    this->m_text = new Text { m_message };
    (*m_text)
        .correct_position(true)
        .border_with_position(true)
        .set_window_size(m_window->getSize())
        .to_center(Element::XY)
        .move(Element::Y, -30)
        .build();

    this->m_button = new Button { {100, 50}, "Ok" };
    (*m_button)
        .correct_position(true)
        .border_with_position(true)
        .set_window_size(m_window->getSize())
        .to_center(Element::X)
        .set_after(Element::Y, *m_text)
        .move(Element::Y, 20)
        .build();

    auto p_window = m_window;

    m_button->on_click(
      [](Button& button) {
        button
            .set_bg_color(sf::Color::Black)
            .set_text_color(sf::Color::White)
            .set_border_color(sf::Color::Red)
            .build();
      },
      [p_window](Button& button) {
          p_window->close();
      }
    );

    m_button->on_hover(
       [](Button& button) {
           button
             .set_border_width(2)
             .build();
       },
       [](Button& button) {
           button
             .set_border_width(0)
             .build();
       }
    );
}

void ErrorForm::draw() {
    m_text->draw(*m_window);
    m_button->draw(*m_window);
}

void ErrorForm::pollEvent(const sf::Event& event) {
    if(event.type == sf::Event::MouseMoved) {
        m_prev_pos = m_curr_pos;
        m_curr_pos = sf::Mouse::getPosition(*m_window);
    }

    Button::input(*m_button, *m_window, event, m_prev_pos);
}

// PUBLIC SET
void ErrorForm::render(float fps, frame_cb_t& frame_cb) {
    m_prev_pos = sf::Mouse::getPosition(*m_window);
    m_curr_pos = sf::Mouse::getPosition(*m_window);

    Form::render(fps, frame_cb);
}
