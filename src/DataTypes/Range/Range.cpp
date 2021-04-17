//
// Created by user on 17.04.2021.
//

#include "Range.h"

Range::Range(size_t start, size_t end) {
    if(start >= end) {
        throw std::runtime_error("start should be more end");
    }

    m_start = start;
    m_end = end;
}

// PUBLIC VOID
void Range::execute(const Range::t_cb& cb) const {
    for(size_t i = m_start; i <= m_end; i++) {
        cb(i);
    }
}

void Range::for_each(size_t start, size_t end, const Range::t_cb& cb) {
    Range(start, end).execute(cb);
}

void Range::for_each(const Range& range, const Range::t_cb& cb) {
    range.execute(cb);
}

// PUBLIC GET
size_t Range::get_start() const {
    return m_start;
}

size_t Range::get_end() const {
    return m_end;
}

bool Range::in_range(size_t val) const {
    return (m_start <= val && val <= m_end);
}
