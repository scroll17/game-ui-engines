//
// Created by user on 04.04.2021.
//

#ifndef STUD_GAME_FOCUSOBJECT_H
#define STUD_GAME_FOCUSOBJECT_H

class FocusObject {
    protected:
        bool m_focus = false;

    public:
        FocusObject() = default;
        virtual ~FocusObject() = default;

        virtual void set_focus();
        virtual void delete_focus();
        virtual bool is_focused() const;
};

#endif //STUD_GAME_FOCUSOBJECT_H
