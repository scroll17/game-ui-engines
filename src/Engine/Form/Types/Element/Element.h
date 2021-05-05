//
// Created by user on 17.02.2021.
//

#ifndef STUD_GAME_ELEMENT_H
#define STUD_GAME_ELEMENT_H

#include <SFML/Graphics.hpp>
#include "../../../DataTypes/Exception/Exception.h"

using namespace data_types;

namespace form {
    namespace types {
        struct Indentation {
            int left;
            int right;

            int top;
            int bottom;
        };


        class Element {
            public:
                enum Axis: int8_t {
                    X = 1,
                    Y = 2,
                    XY = 3
                };

                enum Angle: int8_t {
                    Top = 1,
                    Down = 2,
                    Left = 3,
                    Right = 4
                };

            protected:
                int m_build_count = 0;

                bool m_need_build { true };
                bool m_need_correct_pos { false };
                bool m_border_with_position { false };

                float m_border_width { 0 };

                sf::Vector2u m_window_size {0, 0 };
                sf::Vector2f m_origin { 0, 0 };
                sf::FloatRect m_bounds { 0, 0, 0, 0 };

                sf::Color m_border_color {};

                void turn_on_building();
                void turn_off_building();

                void calculate_position_with_border(const float& prev_width, const float& next_width);

                void change_position(const Axis& axis, const float& pixels);
                void correct_position(const Axis& axis);

                void set_origin_to_center(const Axis& axis, const sf::FloatRect *el_rect = nullptr);

            public:
                explicit Element(const sf::Vector2f& size);
                explicit Element(const sf::Vector2f& size, const sf::Vector2f& position);
                explicit Element(const Element& element);
                virtual ~Element() = default;

                virtual Element& build() = 0;
                virtual void draw(sf::RenderWindow&) const = 0;

                Element& set_size(const sf::Vector2f& size);
                Element& set_width(const float& size);
                Element& set_height(const float& size);
                virtual Element& set_window_size(const sf::Vector2u& size, bool correct_pos = false);

                Element& set_border_width(const float& w);
                Element& set_border_color(const sf::Color& color);
                Element& border_with_position(bool);

                Element& correct_position(bool);
                Element& set_position(const sf::Vector2f& position);

                Element& set_after(const Axis& axis, const Element& el);
                Element& set_before(const Axis& axis, const Element& el);
                Element& set_after_position(const Axis& axis, const sf::Vector2f& position);
                Element& set_before_position(const Axis& axis, const sf::Vector2f& position);

                Element& to_center();
                Element& to_center(const Axis& axis);

                Element& to(const Angle& angel);
                Element& move(const Axis& axis, const float& pixels);

                [[nodiscard]] const sf::FloatRect& get_bounds() const;
                [[nodiscard]] const sf::Vector2f& get_origin() const;

                [[nodiscard]] sf::Vector2f get_size() const;
                [[nodiscard]] sf::Vector2f get_position() const;

                [[nodiscard]] const float& get_x() const;
                [[nodiscard]] const float& get_y() const;
                [[nodiscard]] const float& get_width() const;
                [[nodiscard]] const float& get_height() const;

                static void to_center(Element& current, Element& target, const Indentation *indentation = nullptr);
        };

    }
}

#endif //STUD_GAME_ELEMENT_H
