//
// Created by user on 04.04.2021.
//

#include "FocusObject.h"

using namespace form::objects;

void FocusObject::set_focus() {
    m_focus = true;
}

void FocusObject::delete_focus() {
    m_focus = false;
}

bool FocusObject::is_focused() const {
    return m_focus;
}
