//
// Created by user on 05.04.2021.
//

#ifndef STUD_GAME_ACTIONOBJECT_H
#define STUD_GAME_ACTIONOBJECT_H

#include <vector>     // vector
#include <functional> // function

#include "../../../DataTypes/Exception/Exception.h"
#include "../../Types/Element/Element.h"

class ActionObject {
    public:
        using t_callback = std::function<void(Element&)>;
        using t_callbacks = std::vector<t_callback>;

        enum Action: int8_t {
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

        virtual void call_callbacks(const Action& action) = 0;
        virtual void call_after_callbacks(const Action& action) = 0;

        virtual void remove_callback(const Action& action, size_t pos);

    public:
        ActionObject() = default;
        virtual ~ActionObject() = default;

        virtual size_t on_click(const t_callback& cb, const t_callback& after_cb);
        virtual size_t on_hover(const t_callback& cb, const t_callback& after_cb);
        virtual size_t on_focus(const t_callback& cb, const t_callback& after_cb);

        virtual ActionObject& set_focus();
        virtual ActionObject& delete_focus();

        virtual ActionObject& click();
        virtual ActionObject& after_click();

        virtual ActionObject& hover();
        virtual ActionObject& after_hover();

        virtual ActionObject& remove_click(size_t pos);
        virtual ActionObject& remove_hover(size_t pos);
        virtual ActionObject& remove_focus(size_t pos);

        bool is_focused() const;
};


#endif //STUD_GAME_ACTIONOBJECT_H
