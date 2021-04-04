//
// Created by user on 04.04.2021.
//

#ifndef STUD_GAME_FOCUSELEMENT_H
#define STUD_GAME_FOCUSELEMENT_H

#include "../../Objects/FocusObject/FocusObject.h"
#include "../../Types/Element/Element.h"

class FocusElement: public Element, public FocusObject {
    public:
        explicit FocusElement(const sf::Vector2f& size): Element(size) {};
        explicit FocusElement(const sf::Vector2f& size, const sf::Vector2f& position): Element(size, position) {};
        virtual ~FocusElement() = default;
};

#endif //STUD_GAME_FOCUSELEMENT_H
