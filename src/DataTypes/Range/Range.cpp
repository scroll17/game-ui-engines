//
// Created by user on 17.04.2021.
//

#include "Range.h"

using namespace data_types;

Range::Range(size_t start, size_t end, bool include_end) {
    m_start = std::min(end, start);
    m_end = std::max(end, start);

    m_include_end = include_end;
}

// PUBLIC VOID
void Range::execute(const Range::t_cb& cb) const {
    size_t pos = m_start;
    size_t end = m_include_end ? m_end : m_end - 1;

    while (pos <= end) {
        cb(pos++);
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

// PUBLIC SET
void Range::include_end(bool solution) {
    m_include_end = solution;
}