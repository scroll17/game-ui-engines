//
// Created by user on 16.02.2021.
//

#include "Form.h"

using namespace form;
using namespace form::types;

// PROTECTED SET
void Form::turn_build_on() {
    m_need_build = false;
}

void Form::correct_fps() {
    sf::Int32 frame_duration = m_loop_timer.getElapsedTime().asMilliseconds();
    sf::Int32 time_to_sleep = static_cast<int>(1000.f / m_fps) - frame_duration;

    if (time_to_sleep > 0) {
        sf::sleep(sf::milliseconds(time_to_sleep));
    }

    m_loop_timer.restart();
}

// PROTECTED GET
bool Form::is_need_build() const {
    return m_need_build;
}

bool Form::empty_frame_cb(sf::RenderWindow& w) {
    return true;
}

// PUBLIC GET
void Form::render(float fps, t_frame_cb& frame_cb) {
    if(m_window == nullptr) throw std::runtime_error("Cannot render empty window");
    if(this->is_need_build()) this->build();

    m_fps = fps;
    m_loop_timer.restart();

    sf::Event event {};
    while (m_window->isOpen()) {
        auto need_render = frame_cb(*m_window);
        if(!need_render) return;

        while (m_window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) m_window->close();

            this->pollEvent(event);
        }

        m_window->clear(sf::Color(250, 220, 100, 0));
        this->draw();
        m_window->display();

        this->correct_fps();
    }
}

// PUBLIC STATIC GET
bool Form::mouse_in(const Element& el, const sf::RenderWindow& window) {
    return Form::mouse_in(el, sf::Mouse::getPosition(window));
}

bool Form::mouse_in(const Element& el, const sf::Vector2i& mouse_pos) {
    auto& bounds = el.get_bounds();
    auto& origin = el.get_origin();

    bool x_in = (mouse_pos.x >= (bounds.left - origin.x)) && (mouse_pos.x <= (bounds.left + origin.x));
    bool y_in = (mouse_pos.y >= (bounds.top - origin.y)) && (mouse_pos.y <= (bounds.top + origin.y));

    return x_in && y_in;
}