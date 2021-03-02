//
// Created by user on 09.02.2021.
//

#ifndef STUD_GAME_BUTTON_H
#define STUD_GAME_BUTTON_H

#include <string>
#include <vector>     // vector
#include <functional> // function

#include <SFML/Graphics.hpp>
#include <utility>

//#include "../../../Exception/Exception.h"

#include "../../../Game.h"
#include "../../Form.h"
#include "../Element/Element.h"

using namespace std;

class Button: public Element {
    using t_callback = function<void(Button&)>;
    using t_callbacks = vector<t_callback>;

    public:
        enum Action {
            Click,
            Hover,
            ClickReleased,
            HoverReleased
        };

    protected:
        string m_str;
        sf::Color m_bg_color { sf::Color::White };

        int m_text_size { 18 };
        sf::Color m_text_color { sf::Color::Black };

        t_callbacks m_on_click_callbacks {};
        t_callbacks m_on_hover_callbacks {};

        sf::RectangleShape *m_button { nullptr };
        sf::Text *m_text { nullptr };

        void button_text_to_center();

        void call_callbacks(const Action& action);
        void call_after_callbacks(const Action& action);
        void remove_callback(const Action& action, int pos);

        t_callbacks& get_callbacks_by_action(const Action& action);

    public:
        explicit Button(const sf::Vector2f& size, const string& str);
        explicit Button(const sf::Vector2f& size, const sf::Vector2f& pos, const string& str);
        ~Button() override;

        virtual Button& build() override;
        virtual void draw(sf::RenderWindow& window) override;

        Button& set_text(const string& text);
        Button& set_text_size(int size);
        Button& set_text_color(const sf::Color& color);
        Button& set_bg_color(const sf::Color& color);

        int on_click(const t_callback& cb, const t_callback& after_cb);
        int on_hover(const t_callback& cb, const t_callback& after_cb);

        Button& click();
        Button& after_click();

        Button& hover();
        Button& after_hover();

        Button& remove_click(int pos);
        Button& remove_hover(int pos);

        static void perform_button_actions(Button& button, const sf::RenderWindow& window, const sf::Event& event, const sf::Vector2i& prev_pos);
};

#endif //STUD_GAME_BUTTON_H
