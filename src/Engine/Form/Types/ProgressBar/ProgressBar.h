//
// Created by user on 09.02.2021.
//

#ifndef STUD_GAME_PROGRESSBAR_H
#define STUD_GAME_PROGRESSBAR_H

#include <functional> // function

#include "../../../../DataTypes/Exception/Exception.h"
#include "../Element/Element.h"

namespace form {
    namespace types {

        class ProgressBar: public Element {
            using t_callback = function<void()>;

            protected:
                using Element::set_size;

            protected:
                float m_curr_step;
                const float m_original_width, m_step_in_pixels, m_step_count;

                sf::Color m_color { sf::Color::Blue };
                sf::RectangleShape *m_rectangle { nullptr };

                bool m_callback_was_called { false };
                t_callback m_callback {};

                bool is_end() const;
                void call_callback();

                virtual ProgressBar& build() override;

            public:
                explicit ProgressBar(const sf::Vector2f& size, float step_count);
                explicit ProgressBar(float height, float start_width, float end_width, float step_count);
                ~ProgressBar() override;

                virtual void draw(sf::RenderWindow& window) const override;

                ProgressBar& set_color(const sf::Color& color);

                bool next();
                bool next(float step);

                void ready();
                void after_ready(const t_callback& after_cb);

//        static void auto_progress(ProgressBar& bar, long seconds);
        };

    }
}


#endif //STUD_GAME_PROGRESSBAR_H
