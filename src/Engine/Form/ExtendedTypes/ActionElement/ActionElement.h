//
// Created by user on 06.04.2021.
//

#ifndef STUD_GAME_ACTIONELEMENT_H
#define STUD_GAME_ACTIONELEMENT_H

#include "../../Objects/ActionObject/ActionObject.h"
#include "../../Types/Element/Element.h"

template <class T>
class ActionElement: public Element, public ActionObject<T>  {
    public:
        explicit ActionElement(const sf::Vector2f& size): Element(size) {};
        explicit ActionElement(const sf::Vector2f& size, const sf::Vector2f& position): Element(size, position) {};
        virtual ~ActionElement() = default;
};

#endif //STUD_GAME_ACTIONELEMENT_H
