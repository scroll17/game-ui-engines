//
// Created by user on 08.02.2021.
//

#ifndef STUD_GAME_INTERFACEFORM_H
#define STUD_GAME_INTERFACEFORM_H

#include <functional> // function
#include <string>

#include "./Types/Button/Button.h"

// TODO _Abstract class
// TODO _need 5 forms of interface

using namespace std;

class FormInterface {
    public:
//        InterfaceForm() = default;
//        virtual ~InterfaceForm() = default;
//
//        virtual void show() = 0;
//        virtual void hide() = 0;
//
//        virtual void onClose() = 0;
//
//        virtual void render(int frequency) {
//            // ...
//            this->draw();
//        }

        static bool mouse_in(const Element& el, const sf::RenderWindow& window);
        static bool mouse_in(const Element& el, const sf::Vector2i& mouse_pos);

        static float get_center(const sf::RenderWindow& window);

//    protected:
//        virtual void draw() = 0;
};


#endif //STUD_GAME_INTERFACEFORM_H
