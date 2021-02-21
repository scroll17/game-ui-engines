//
// Created by user on 09.02.2021.
//

#ifndef STUD_GAME_ELEMENT_H
#define STUD_GAME_ELEMENT_H

#include <SFML/Graphics.hpp>
#include "iostream"

class Element {
    protected:
        bool m_builded { false };

        bool m_actual_x_bounds { false };
        bool m_actual_y_bounds { false };

        bool m_position_with_border {false };

        sf::Vector2f m_size;
        sf::Vector2f m_position { 0, 0 };
        sf::Vector2f m_origin { 0, 0 };

        sf::FloatRect m_bounds { 0, 0, 0, 0 };

        float m_border_width {};
        sf::Color m_border_color {};

        virtual Element& build() = 0;
        virtual void draw(sf::RenderWindow&) = 0;

        explicit Element(): m_size(0, 0) {};

        void turn_off_building();
        void turn_on_building();

        void turn_on_boundary(int);
        void turn_off_boundary(int);

        void calculate_position_with_border(float prev_with, float next_with);

        void calculate_bounds();
        void calculate_bounds_x();
        void calculate_bounds_y();

        void set_correct_origin(bool use_x = true, bool use_y = true, const sf::FloatRect *el_rect = nullptr);

        void origin_to_center();
        void origin_to_center(const sf::FloatRect& el_rect);
        void origin_x_to_center(const sf::FloatRect& el_rect);
        void origin_y_to_center(const sf::FloatRect& el_rect);
        void origin_x_to_center(const float& left, const float& width);
        void origin_y_to_center(const float& top, const float& height);

    public:
        explicit Element(const sf::Vector2f& size): m_size(size) {};
        virtual ~Element() = default;

        Element& set_size(const sf::Vector2f& size);

        Element& set_border_width(float w);
        Element& set_border_color(const sf::Color& color);
        Element& calculate_position_with_border(bool);

        Element& set_position(const sf::Vector2f& position);
        Element& set_after_y_position(const Element& el);
        Element& set_after_x_position(const Element& el);
        Element& set_before_y_position(const Element& el);
        Element& set_before_x_position(const Element& el);

        Element& to_center(const sf::Vector2u& window_size);
        Element& to_center_x(const sf::Vector2u& window_size);
        Element& to_center_y(const sf::Vector2u& window_size);

        Element& to_left_top(const sf::Vector2u& window_size);
        Element& to_left_down(const sf::Vector2u& window_size);

        Element& to_right_top(const sf::Vector2u& window_size);
        Element& to_right_down(const sf::Vector2u& window_size);

        Element& moving_by_x(float val);
        Element& moving_by_y(float val);

        [[nodiscard]] const sf::Vector2f& get_position() const;
        [[nodiscard]] const sf::Vector2f& get_origin() const;
        [[nodiscard]] const sf::Vector2f& get_size() const;

        float get_height() const;
        float get_width() const;

        [[nodiscard]] virtual sf::FloatRect get_bounds() const;
        //[[nodiscard]] virtual const sf::FloatRect& get_bounds() const;
};

#endif //STUD_GAME_ELEMENT_H
