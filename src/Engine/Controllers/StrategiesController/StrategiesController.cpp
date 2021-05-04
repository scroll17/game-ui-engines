//
// Created by user on 03.05.2021.
//

#include "StrategiesController.h"

/// PUBLIC VOID
void StrategiesController::start() {
    m_active = true;

    auto iter = 0;
    auto it = std::begin(m_strategies) + m_current_strategy;
    while (it != std::end(m_strategies)) {
        bool result = (*it)();
        if(!result) {
            break;
        }

        if(iter != m_current_strategy) {
            it = std::begin(m_strategies) + m_current_strategy;
            iter = m_current_strategy;
            continue;
        }

        if(!this->m_active) {
            m_current_strategy++;
            break;
        }

        it++;
        iter++;
        m_current_strategy++;

        if(m_current_strategy == m_strategies.size() - 1) {
            m_current_strategy = 0;
        }
    }
}

void StrategiesController::start(size_t pos) {
    auto it = m_strategies.at(pos);
    it();
}

void StrategiesController::stop() {
    m_active = false;
}

/// PUBLIC SET
size_t StrategiesController::register_strategy(const StrategiesController::t_cb& cb) {
    m_strategies.push_back(cb);
    return m_strategies.size() - 1;
}

void StrategiesController::next(size_t pos) {
    m_current_strategy = pos;
}

/// PUBLIC GET
size_t StrategiesController::get_current_strategy() const {
    return m_current_strategy;
}
