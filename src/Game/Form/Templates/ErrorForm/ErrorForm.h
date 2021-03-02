//
// Created by user on 09.02.2021.
//

#ifndef STUD_GAME_ERRORFORM_H
#define STUD_GAME_ERRORFORM_H

#include "SFML/Graphics.hpp"

#include "../../Form.h"

class ErrorForm final: public Form {
    private:


    protected:
        virtual void build() override;

        virtual void draw() override;
        virtual void pollEvent(const sf::Event& event) override;

    public:
        ErrorForm();
        ErrorForm(const sf::ContextSettings& settings);
        ~ErrorForm() override;
};

void test() {
    ErrorForm form;
    form.render(25);
}


#endif //STUD_GAME_ERRORFORM_H
