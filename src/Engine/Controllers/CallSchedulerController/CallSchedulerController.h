//
// Created by user on 10.04.2021.
//

#ifndef STUD_GAME_CALLSCHEDULERCONTROLLER_H
#define STUD_GAME_CALLSCHEDULERCONTROLLER_H

#include <map>

#include "./SchedulerPlan/SchedulerPlan.h"
#include "../../../utils/time/time.h"

class CallSchedulerController final {
    public:
        using t_plan = std::pair<size_t, SchedulerPlan*>;
        using t_plans = std::map<size_t, SchedulerPlan*>;

    protected:
        static CallSchedulerController *s_m_instance;

        size_t m_last_id = 0;
        t_plans *m_plans { nullptr };

        CallSchedulerController();
        ~CallSchedulerController();

    public:
        CallSchedulerController(CallSchedulerController& other) = delete;
        void operator=(const CallSchedulerController&) = delete;

        SchedulerPlan& schedule(size_t every_ms, SchedulerPlan::t_cb&& cb);
        SchedulerPlan& schedule(size_t every_ms, size_t count, SchedulerPlan::t_cb&& cb);

        SchedulerPlan* get_scheduled_plan(size_t id) const;
        bool remove_scheduled_plan(size_t id);

        void call();

        static CallSchedulerController& get_instance();
        static void remove_instance();
};

#endif //STUD_GAME_CALLSCHEDULERCONTROLLER_H
