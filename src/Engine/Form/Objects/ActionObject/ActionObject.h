//
// Created by user on 05.04.2021.
//

#ifndef STUD_GAME_ACTIONOBJECT_H
#define STUD_GAME_ACTIONOBJECT_H

#include <vector>     // vector
#include <functional> // function

#include "../../../Exception/Exception.h"

template <class T>
class ActionObject {
    using t_callback = std::function<void(T&)>;
    using t_callbacks = std::vector<t_callback>;

    public:
        enum Action {
            Click,
            Hover,
            Focus
        };

    protected:
        bool m_focus = false;

        t_callbacks m_on_click_callbacks {};
        t_callbacks m_on_hover_callbacks {};
        t_callbacks m_on_focus_callbacks {};

        size_t set_callbacks_by_action(const Action& action, const t_callback& cb, const t_callback& after_cb);
        t_callbacks& get_callbacks_by_action(const Action& action);

        void call_callbacks(const Action& action);
        void call_after_callbacks(const Action& action);
        void remove_callback(const Action& action, size_t pos);

    public:
        ActionObject() = default;
        virtual ~ActionObject() = default;

        size_t on_click(const t_callback& cb, const t_callback& after_cb);
        size_t on_hover(const t_callback& cb, const t_callback& after_cb);
        size_t on_focus(const t_callback& cb, const t_callback& after_cb);

        ActionObject& set_focus();
        ActionObject& delete_focus();

        ActionObject& click();
        ActionObject& after_click();

        ActionObject& hover();
        ActionObject& after_hover();

        ActionObject& remove_click(size_t pos);
        ActionObject& remove_hover(size_t pos);
        ActionObject& remove_focus(size_t pos);

        bool is_focused() const;
};


#endif //STUD_GAME_ACTIONOBJECT_H
