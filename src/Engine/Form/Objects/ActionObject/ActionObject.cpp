//
// Created by user on 05.04.2021.
//

#include "ActionObject.h"

using namespace form::objects;

// PROTECTED GET
ActionObject::t_callbacks& ActionObject::get_callbacks_by_action(const Action& action) {
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
size_t ActionObject::set_callbacks_by_action(const Action &action, const t_callback &cb, const t_callback &after_cb) {
    t_callbacks& callbacks(get_callbacks_by_action(action));

    callbacks.push_back(cb);
    size_t pos = callbacks.size() - 1;

    callbacks.push_back(after_cb);

    return pos / 2;
}

void ActionObject::remove_callback(const Action& action, size_t pos) {
    if(pos % 2 != 0) throw std::runtime_error("Invalid argument");

    t_callbacks& callbacks(get_callbacks_by_action(action));

    if(pos > (callbacks.size() - 2)) {
        throw data_types::Exception(data_types::Exception::NonExistentPosition);
    }

    callbacks.erase(callbacks.begin() + pos);               /// on callback
    callbacks.erase(callbacks.begin() + pos + 1);   /// after callback
}

// PUBLIC SET
size_t ActionObject::on_click(const ActionObject::t_callback& cb, const ActionObject::t_callback& after_cb) {
    return this->set_callbacks_by_action(Action::Click, cb, after_cb);
}

size_t ActionObject::on_hover(const ActionObject::t_callback& cb, const ActionObject::t_callback& after_cb) {
    return this->set_callbacks_by_action(Action::Hover, cb, after_cb);
}

size_t ActionObject::on_focus(const ActionObject::t_callback &cb, const ActionObject::t_callback &after_cb) {
    return this->set_callbacks_by_action(Action::Focus, cb, after_cb);
}

ActionObject& ActionObject::set_focus() {
    m_focus = true;
    this->call_callbacks(Action::Focus);

    return (*this);
}

ActionObject& ActionObject::delete_focus() {
    m_focus = false;
    this->call_after_callbacks(Action::Focus);

    return (*this);
}

ActionObject& ActionObject::click() {
    this->call_callbacks(Action::Click);
    return (*this);
}

ActionObject& ActionObject::after_click() {
    this->call_after_callbacks(Action::Click);
    return (*this);
}

ActionObject& ActionObject::hover() {
    this->call_callbacks(Action::Hover);
    return (*this);
}

ActionObject& ActionObject::after_hover() {
    this->call_after_callbacks(Action::Hover);
    return (*this);
}

ActionObject& ActionObject::remove_click(size_t pos) {
    remove_callback(Action::Click, pos * 2);
    return (*this);
}

ActionObject& ActionObject::remove_hover(size_t pos) {
    remove_callback(Action::Hover, pos * 2);
    return (*this);
}

ActionObject& ActionObject::remove_focus(size_t pos) {
    remove_callback(Action::Focus, pos * 2);
    return (*this);
}

// PUBLIC GET
bool ActionObject::is_focused() const {
    return m_focus;
}
