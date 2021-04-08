//
// Created by user on 06.04.2021.
//

#ifndef STUD_GAME_ACTIONELEMENT_H
#define STUD_GAME_ACTIONELEMENT_H

#include "../../Objects/ActionObject/ActionObject.h"
#include "../../Types/Element/Element.h"

class ActionElement: public Element, public ActionObject  {
    public:
        explicit ActionElement(const sf::Vector2f& size): Element(size) {};
        explicit ActionElement(const sf::Vector2f& size, const sf::Vector2f& position): Element(size, position) {};
        virtual ~ActionElement() = default;

        void call_callbacks(const Action& action) override;
        void call_after_callbacks(const Action& action) override;
};

#endif //STUD_GAME_ACTIONELEMENT_H
