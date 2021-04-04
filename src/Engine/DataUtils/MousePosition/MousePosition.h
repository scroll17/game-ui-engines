//
// Created by user on 04.04.2021.
//

#ifndef STUD_GAME_MOUSEPOSITION_H
#define STUD_GAME_MOUSEPOSITION_H

#include "SFML/Graphics.hpp"

class MousePosition {
    protected:
        sf::Vector2i m_prev_pos {};
        sf::Vector2i m_curr_pos {};

    public:
        explicit MousePosition(const sf::RenderWindow& w);
        ~MousePosition() = default;

        MousePosition& input(const sf::RenderWindow& w, const sf::Event& event);

        const sf::Vector2i& get_prev_pos() const;
        const sf::Vector2i& get_curr_pos() const;
};


#endif //STUD_GAME_MOUSEPOSITION_H
