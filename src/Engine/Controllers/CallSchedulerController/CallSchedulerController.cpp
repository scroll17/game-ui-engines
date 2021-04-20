//
// Created by user on 10.04.2021.
//

#include "CallSchedulerController.h"

using namespace engine::controllers;

CallSchedulerController *CallSchedulerController::s_m_instance = nullptr;;

CallSchedulerController::CallSchedulerController() {
    m_plans = new CallSchedulerController::t_plans {};
}

CallSchedulerController::~CallSchedulerController() {
    for(auto& el : *m_plans) {
        delete el.second;
    }

    delete m_plans;
}

// PUBLIC GET
SchedulerPlan* CallSchedulerController::get_scheduled_plan(size_t id) const {
    auto pos = m_plans->find(id);
    if(pos != std::end(*m_plans)) {
        return pos->second;
    }

    return nullptr;
}

// PUBLIC SET
CallSchedulerController& CallSchedulerController::get_instance() {
    if(s_m_instance == nullptr) {
        return *(s_m_instance = new CallSchedulerController());
    }

    return *s_m_instance;
}

void CallSchedulerController::remove_instance() {
    delete s_m_instance;
}

SchedulerPlan& CallSchedulerController::schedule(size_t every_ms, SchedulerPlan::t_cb&& cb) {
    ++m_last_id;

    auto plan = (*m_plans)[m_last_id] = new SchedulerPlan(m_last_id, every_ms, std::move(cb));

    return (*plan);
}

SchedulerPlan& CallSchedulerController::schedule(size_t every_ms, size_t count, SchedulerPlan::t_cb&& cb) {
    ++m_last_id;

    auto plan = (*m_plans)[m_last_id] = new SchedulerPlan(m_last_id, every_ms, count, std::move(cb));

    return (*plan);
}

bool CallSchedulerController::remove_scheduled_plan(size_t id) {
    auto pos = m_plans->find(id);
    if(pos != std::end(*m_plans)) {
        delete pos->second;
        m_plans->erase(pos);

        return true;
    }

    return false;
}

void CallSchedulerController::call() {
    const auto& time_now = utils::time::get_timestamp();

    std::for_each(
        std::begin(*m_plans),
        std::end(*m_plans),
        [&time_now](const t_plan& pair) {
            pair.second->execute(time_now);
        }
    );
}