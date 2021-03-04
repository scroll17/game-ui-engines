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

using namespace std;

class ErrorForm final: public Form {
    private:
        string m_message;

        Text *m_text { nullptr };
        Button *m_button { nullptr };

        sf::Vector2i m_prev_pos {};
        sf::Vector2i m_curr_pos {};

    protected:
        using Form::render;

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
