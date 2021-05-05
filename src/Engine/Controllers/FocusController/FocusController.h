//
// Created by user on 04.04.2021.
//

#ifndef STUD_GAME_FOCUSCONTROLLER_H
#define STUD_GAME_FOCUSCONTROLLER_H

#include <utility>
#include <map>
#include <set>
#include "SFML/Graphics.hpp"

#include "../../Form/Form.h"
#include "../../DataUtils/MousePosition/MousePosition.h"
#include "../../../DataTypes/Exception/Exception.h"
#include "../../Form/ExtendedTypes/ActionElement/ActionElement.h"

using namespace form::extended_types;
using namespace data_types;

namespace engine {

    namespace controllers {

        class FocusController final {
            public:
                using t_elements = std::set<ActionElement*>;
                using t_window_elements = std::map<const sf::RenderWindow*, t_elements*>;

            protected:
                static FocusController *s_m_instance;

                sf::RenderWindow *m_curr_window { nullptr };

                t_window_elements *m_window_elements { nullptr };
                t_elements *m_elements { nullptr };

                FocusController();
                ~FocusController();

            public:
                FocusController(FocusController& other) = delete;
                void operator=(const FocusController&) = delete;

                FocusController& register_element(ActionElement *el);
                FocusController& remove_registration(ActionElement *el);

                FocusController& set_window(sf::RenderWindow *w);
                FocusController& input(const sf::Event& event);

                ActionElement *get_focused_el() const;
                sf::RenderWindow *get_focused_window() const;

                static FocusController& get_instance();
                static void remove_instance();
        };

    }

}

#endif //STUD_GAME_FOCUSCONTROLLER_H