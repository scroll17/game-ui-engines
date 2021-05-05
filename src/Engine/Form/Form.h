//
// Created by user on 08.02.2021.
//

#ifndef STUD_GAME_INTERFACEFORM_H
#define STUD_GAME_INTERFACEFORM_H

#include "iostream"
#include <chrono>
#include <ctime>

#include <functional> // function
#include <string>
#include "SFML/Graphics.hpp"

#include "./Types/Element/Element.h"
#include "../../DataTypes/Exception/Exception.h"

using namespace std;
using namespace data_types;

namespace form {

    class Form {
        private:
            bool m_need_build { true };

        protected:
            float m_fps { 60 };

            sf::Clock m_loop_timer;
            sf::RenderWindow *m_window { nullptr };

            virtual void draw() = 0;
            virtual void build() = 0;

            virtual void pollEvent(const sf::Event& event) = 0;

            void correct_fps();

            void turn_build_on();
            bool is_need_build() const;

        public:
            using t_frame_cb = const function<bool(sf::RenderWindow&)>;

            Form() = default;
            virtual ~Form() = default;

            virtual void render(float fps, t_frame_cb& frame_cb);

            static bool mouse_in(const types::Element& el, const sf::RenderWindow& window);
            static bool mouse_in(const types::Element& el, const sf::Vector2i& mouse_pos);

            static bool empty_frame_cb(sf::RenderWindow&);
    };

}


#endif //STUD_GAME_INTERFACEFORM_H
