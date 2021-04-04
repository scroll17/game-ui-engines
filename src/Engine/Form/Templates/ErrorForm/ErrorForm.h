//
// Created by user on 09.02.2021.
//

#ifndef STUD_GAME_ERRORFORM_H
#define STUD_GAME_ERRORFORM_H

#include <utility>
#include "SFML/Graphics.hpp"

#include "../../Form.h"
#include "../../Types/Text/Text.h"
#include "../../Types/Button/Button.h"
#include "../../../DataUtils/MousePosition/MousePosition.h"

using namespace std;

class ErrorForm final: public Form {
    private:
        string m_message;

        Text *m_text { nullptr };
        Button *m_button { nullptr };

        MousePosition m_mouse_position {};

    protected:
        virtual void build() override;

        virtual void draw() override;
        virtual void pollEvent(const sf::Event& event) override;

    public:
        ErrorForm(string message);
        ErrorForm(string message, const sf::ContextSettings& settings);
        ~ErrorForm() override;

        virtual void render(float fps, t_frame_cb& frame_cb) override;
};

#endif //STUD_GAME_ERRORFORM_H
