//
// Created by user on 11.01.2021.
//

#include "Time.h"

using namespace data_types;

void Timer::reset() {
    m_beg = clock_t::now();
}

double Timer::elapsed() const {
    return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
}
