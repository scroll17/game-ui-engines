//
// Created by user on 09.02.2021.
//

#include "ErrorForm.h"

ErrorForm::ErrorForm(string message): m_message(std::move(message)) {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    m_window = new sf::RenderWindow(
      sf::VideoMode(450, 150),
      "Error",
      sf::Style::Default,
      settings
    );
    m_mouse_position = &MousePosition::get_instance(m_window);
}

ErrorForm::ErrorForm(string message, const sf::ContextSettings& settings): m_message(std::move(message)) {
    m_window = new sf::RenderWindow(
      sf::VideoMode(450, 150),
      "Error",
      sf::Style::Default,
      settings
    );
    m_mouse_position = &MousePosition::get_instance(m_window);
}

ErrorForm::~ErrorForm() {
    MousePosition::remove_instance(m_mouse_position);

    delete m_window;
    delete m_text;
    delete m_button;
}

// PROTECTED SET
void ErrorForm::build() {
    m_text = new Text { m_message };
    (*m_text)
        .correct_position(true)
        .border_with_position(true)
        .set_window_size(m_window->getSize())
        .to_center(Element::XY)
        .move(Element::Y, -30)
        .build();

    m_button = new Button { {100, 50}, "Ok" };
    (*m_button)
        .correct_position(true)
        .border_with_position(false)
        .set_window_size(m_window->getSize())
        .to_center(Element::X)
        .set_after(Element::Y, *m_text)
        .move(Element::Y, 20)
        .build();

    auto p_window = m_window;

    m_button->on_click(
      [](Element& el) {
        auto button = dynamic_cast<Button&>(el);

        button
            .set_bg_color(sf::Color::Black)
            .set_text_color(sf::Color::White)
            .set_border_color(sf::Color::Red)
            .build();
      },
      [p_window](Element& el) {
          p_window->close();
      }
    );

    m_button->on_hover(
       [](Element& el) {
           el
             .set_border_width(2)
             .build();
       },
       [](Element& el) {
           el
             .set_border_width(0)
             .build();
       }
    );

    this->turn_build_on();
}

void ErrorForm::draw() {
    m_text->draw(*m_window);
    m_button->draw(*m_window);
}

void ErrorForm::pollEvent(const sf::Event& event) {
    MousePosition::input(*m_mouse_position, *m_window, event);
    Button::input(*m_button, *m_window, event, m_mouse_position->get_prev_pos());
}

// PUBLIC SET
void ErrorForm::render(float fps, t_frame_cb& frame_cb) {
    m_mouse_position->update_pos(*m_window);

    this->Form::render(fps, frame_cb);
}
