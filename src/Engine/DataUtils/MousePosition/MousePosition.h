//
// Created by user on 04.04.2021.
//

#ifndef STUD_GAME_MOUSEPOSITION_H
#define STUD_GAME_MOUSEPOSITION_H

#include "SFML/Graphics.hpp"
#include <map>

namespace engine {

    namespace data_utils {

        class MousePosition {
            public:
                using t_instance = std::pair<const sf::RenderWindow*, MousePosition*>;
                using t_instances = std::map<const sf::RenderWindow*, MousePosition*>;

            protected:
                static t_instances s_m_instances;

                sf::Vector2i m_prev_pos {};
                sf::Vector2i m_curr_pos {};

                explicit MousePosition();
                explicit MousePosition(const sf::RenderWindow& w);
                ~MousePosition() = default;

            public:
                MousePosition(MousePosition& other) = delete;
                void operator=(const MousePosition&) = delete;

                MousePosition& update_pos(const sf::RenderWindow& w);

                const sf::Vector2i& get_prev_pos() const;
                const sf::Vector2i& get_curr_pos() const;

                static void input(MousePosition& mouse_pos, const sf::RenderWindow& w, const sf::Event& event);

                static MousePosition& get_instance(const sf::RenderWindow* window);
                static bool remove_instance(const sf::RenderWindow* window);
                static bool remove_instance(const MousePosition* m_p);
                static void clear_all();
        };

    }

}

#endif //STUD_GAME_MOUSEPOSITION_H
