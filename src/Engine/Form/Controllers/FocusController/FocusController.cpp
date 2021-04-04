//
// Created by user on 04.04.2021.
//

#include "FocusController.h"

FocusController *FocusController::s_m_instance = nullptr;

FocusController::FocusController() {
    m_window_elements = new FocusController::t_window_elements {};
}

FocusController::~FocusController() {
    for(auto& el : *m_window_elements) {
        delete el.second;
    }

    delete m_window_elements;
}

// PUBLIC SET
FocusController& FocusController::get_instance() {
    if(s_m_instance == nullptr) {
        return *(s_m_instance = new FocusController());
    }

    return *s_m_instance;
}

void FocusController::remove_instance() {
    delete s_m_instance;
}

FocusController& FocusController::register_element(FocusElement *el) {
    if(m_curr_window == nullptr) {
        throw std::runtime_error("Need set window before");
    }

    m_elements->insert(el);

    return (*this);
}

FocusController& FocusController::remove_registration(FocusElement *el) {
    if(m_curr_window == nullptr) {
        throw std::runtime_error("Need set window before");
    }

    auto pos = m_elements->find(el);
    if(pos != std::end(*m_elements)) {
        m_elements->erase(pos);
    }

    return (*this);
}

FocusController& FocusController::set_window(sf::RenderWindow *w) {
    if(m_curr_window != w) {
        m_curr_window = w;

        auto it = m_window_elements->find(w);
        if(it == std::end(*m_window_elements)) {
            m_elements = (*m_window_elements)[w] = new FocusController::t_elements;
        } else {
            m_elements = it->second;
        }
    }

    return (*this);
}

FocusController& FocusController::input(const sf::Event& event) {
    if(m_curr_window == nullptr) {
        throw std::runtime_error("Need set window before");
    }

    if(m_elements->empty()) return (*this);

    if(event.type == sf::Event::MouseButtonPressed) {
        auto is_left_button = event.mouseButton.button == sf::Mouse::Left;
        if(!is_left_button) return (*this);

        for(auto& el: *m_elements) {
            bool el_now_focused = Form::mouse_in(*el, *m_curr_window);
            bool el_status_is_focused = el->is_focused();

            if(el_now_focused && !el_status_is_focused) {
                el->set_focus();
            }
            if(!el_now_focused && el_status_is_focused) {
                el->delete_focus();
            }
        }
    }

    return (*this);
}


// PUBLIC GET
FocusElement *FocusController::get_focused_el() const {
    if(m_curr_window == nullptr) {
        throw std::runtime_error("Need set window before");
    }

    auto it = std::begin(*m_elements);
    while (it != std::end(*m_elements)) {
        if((*it)->is_focused()) return *it;

        ++it;
    }

    return nullptr;
}

sf::RenderWindow *FocusController::get_focused_window() const {
    return m_curr_window;
}