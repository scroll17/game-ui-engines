//
// Created by user on 10.04.2021.
//

#ifndef STUD_GAME_SCHEDULERPLAN_H
#define STUD_GAME_SCHEDULERPLAN_H

#include <limits>
#include <functional>
#include <chrono>

#include "../../../utils/time/time.h"

class CallSchedulerController;

class SchedulerPlan final {
    friend class CallSchedulerController;

    public:
        using t_cb = std::function<void()>;

    private:
        const size_t m_id;
        t_cb m_cb;

        size_t m_every_ms;
        size_t m_last_execute_ts = 0;

        size_t m_count = std::numeric_limits<size_t>::max();
        bool m_active = false;

        explicit SchedulerPlan(size_t id, size_t every_ms, t_cb&& cb);
        explicit SchedulerPlan(size_t id, size_t every_ms, size_t count, t_cb&& cb);
        ~SchedulerPlan() = default;

    public:
        SchedulerPlan(SchedulerPlan& other) = delete;
        void operator=(const SchedulerPlan&) = delete;

        void activate();
        void disable();

        void execute(const long& time_now);

        bool is_active() const;

        size_t get_id() const;
        size_t get_count() const;
        size_t get_last_execute_ts() const;
};

#endif //STUD_GAME_SCHEDULERPLAN_H
