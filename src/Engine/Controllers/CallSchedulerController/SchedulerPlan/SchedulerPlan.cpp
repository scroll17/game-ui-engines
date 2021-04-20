//
// Created by user on 10.04.2021.
//

#include "SchedulerPlan.h"

using namespace engine::controllers;

SchedulerPlan::SchedulerPlan(size_t id, size_t every_ms, SchedulerPlan::t_cb&& cb): m_id(id), m_every_ms(every_ms), m_cb(cb) {
}

SchedulerPlan::SchedulerPlan(size_t id, size_t every_ms, size_t count, SchedulerPlan::t_cb&& cb): m_id(id), m_every_ms(every_ms), m_count(count), m_cb(cb) {
}

// PUBLIC GET
bool SchedulerPlan::is_active() const {
    return m_active;
}

size_t SchedulerPlan::get_id() const {
    return m_id;
}

size_t SchedulerPlan::get_count() const {
    return m_count;
}

size_t SchedulerPlan::get_last_execute_ts() const {
    return m_last_execute_ts;
}

// PUBLIC SET
void SchedulerPlan::activate() {
    if(m_count == 0) return;

    if(!m_active) {
        m_active = true;
        this->execute(utils::time::get_timestamp());
    }
}

void SchedulerPlan::disable() {
    m_active = false;
}

void SchedulerPlan::execute(const long& time_now) {
    if(!m_active) return;

    auto diff = time_now - m_last_execute_ts;
    if(diff >= m_every_ms) {
        m_cb();
        m_last_execute_ts = time_now;

        if(--m_count == 0) this->disable();
    }
}