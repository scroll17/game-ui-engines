//
// Created by user on 08.02.2021.
//

#ifndef STUD_GAME_INTERFACEFORM_H
#define STUD_GAME_INTERFACEFORM_H

#include <functional> // function

// TODO _Abstract class
// TODO _need 5 forms of interface

namespace GameInterface {

}

class InterfaceForm {
    public:
        InterfaceForm() = default;
        ~InterfaceForm() = default;

        virtual void show() = 0;

    protected:
        struct Button {
            int width;
            int height;

            string text;
            function<void()> onClick = nullptr;
        };

        struct Text {
            int width;
            int height;

            string text;
        };

        struct Image {
            int width;
            int height;

            function<void()> onClick = nullptr;
            function<void()> onHover = nullptr;
        };

        enum class ElementType {
            BUTTON,
            TEXT,
            IMAGE
        };

        struct Element {
            Text text;
            Button button;
            Image image;
        };

        // pure virtual functions
        virtual void onShow() = 0;
        virtual void onClose() = 0;

        virtual void addElement(ElementType type, Element el) = 0;
};

#endif //STUD_GAME_INTERFACEFORM_H
