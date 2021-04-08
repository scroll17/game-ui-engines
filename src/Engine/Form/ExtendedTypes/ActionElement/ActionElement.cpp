//
// Created by user on 07.04.2021.
//

#include "ActionElement.h"

void ActionElement::call_callbacks(const Action& action) {
    t_callbacks& callbacks(get_callbacks_by_action(action));

    if(callbacks.empty()) return;

    const size_t size = callbacks.size();
    for(size_t i = 0; i < size; i += 2) {
        auto& callback = callbacks.at(i);
        callback(*this);
    }
}

void ActionElement::call_after_callbacks(const Action& action) {
    t_callbacks& callbacks(get_callbacks_by_action(action));

    if(callbacks.empty()) return;

    const size_t size = callbacks.size();
    for(size_t i = 1; i < size; i += 2) {
        auto& callback = callbacks.at(i);
        callback(*this);
    }
}

