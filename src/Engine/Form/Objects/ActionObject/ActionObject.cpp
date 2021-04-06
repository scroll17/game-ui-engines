//
// Created by user on 05.04.2021.
//

#include "ActionObject.h"

// PROTECTED GET
template <class T>
typename ActionObject<T>::t_callbacks& ActionObject<T>::get_callbacks_by_action(const Action& action) {
    switch (action) {
        case Click: return m_on_click_callbacks;
        case Hover: return m_on_hover_callbacks;
        case Focus: return m_on_focus_callbacks;
        default: {
            throw std::runtime_error("Invalid argument");
        }
    }
}

// PROTECTED SET
template <class T>
size_t ActionObject<T>::set_callbacks_by_action(const Action &action, const t_callback &cb, const t_callback &after_cb) {
    t_callbacks& callbacks(get_callbacks_by_action(action));

    callbacks.push_back(cb);
    size_t pos = callbacks.size() - 1;

    callbacks.push_back(after_cb);

    return pos / 2;
}

template <class T>
void ActionObject<T>::call_callbacks(const Action& action) {
    t_callbacks& callbacks(get_callbacks_by_action(action));

    if(callbacks.empty()) return;

    const size_t size = callbacks.size();
    for(size_t i = 0; i < size; i += 2) {
        auto& callback = callbacks.at(i);
        callback(*this);
    }
}

template <class T>
void ActionObject<T>::call_after_callbacks(const Action& action) {
    t_callbacks& callbacks(get_callbacks_by_action(action));

    if(callbacks.empty()) return;

    const size_t size = callbacks.size();
    for(size_t i = 1; i < size; i += 2) {
        auto& callback = callbacks.at(i);
        callback(*this);
    }
}

template <class T>
void ActionObject<T>::remove_callback(const Action& action, size_t pos) {
    if(pos % 2 != 0) throw std::runtime_error("Invalid argument");

    t_callbacks& callbacks(get_callbacks_by_action(action));

    if(pos > (callbacks.size() - 2)) {
        throw Exception(Exception::NonExistentPosition);
    }

    callbacks.erase(callbacks.begin() + pos);               /// on callback
    callbacks.erase(callbacks.begin() + pos + 1);   /// after callback
}

// PUBLIC SET
template <class T>
size_t ActionObject<T>::on_click(const ActionObject::t_callback& cb, const ActionObject::t_callback& after_cb) {
    return this->set_callbacks_by_action(Action::Click, cb, after_cb);
}

template <class T>
size_t ActionObject<T>::on_hover(const ActionObject::t_callback& cb, const ActionObject::t_callback& after_cb) {
    return this->set_callbacks_by_action(Action::Hover, cb, after_cb);
}

template <class T>
size_t ActionObject<T>::on_focus(const ActionObject::t_callback &cb, const ActionObject::t_callback &after_cb) {
    return this->set_callbacks_by_action(Action::Focus, cb, after_cb);
}

template <class T>
ActionObject<T>& ActionObject<T>::set_focus() {
    m_focus = true;
    this->call_callbacks(Action::Focus);

    return (*this);
}

template <class T>
ActionObject<T>& ActionObject<T>::delete_focus() {
    m_focus = false;
    this->call_after_callbacks(Action::Focus);

    return (*this);
}

template <class T>
ActionObject<T>& ActionObject<T>::click() {
    this->call_callbacks(Action::Click);
    return (*this);
}

template <class T>
ActionObject<T>& ActionObject<T>::after_click() {
    this->call_after_callbacks(Action::Click);
    return (*this);
}

template <class T>
ActionObject<T>& ActionObject<T>::hover() {
    this->call_callbacks(Action::Hover);
    return (*this);
}

template <class T>
ActionObject<T>& ActionObject<T>::after_hover() {
    this->call_after_callbacks(Action::Hover);
    return (*this);
}

template <class T>
ActionObject<T>& ActionObject<T>::remove_click(size_t pos) {
    remove_callback(Action::Click, pos * 2);
    return (*this);
}

template <class T>
ActionObject<T>& ActionObject<T>::remove_hover(size_t pos) {
    remove_callback(Action::Hover, pos * 2);
    return (*this);
}

template <class T>
ActionObject<T>& ActionObject<T>::remove_focus(size_t pos) {
    remove_callback(Action::Focus, pos * 2);
    return (*this);
}

// PUBLIC GET
template <class T>
bool ActionObject<T>::is_focused() const {
    return m_focus;
}
