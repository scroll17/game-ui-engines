//
// Created by user on 30.04.2021.
//

#include "ExtendedRange.h"

using namespace data_types;

ExtendedRange::ExtendedRange(size_t start, size_t end, size_t second_value): Range(start, end, false), m_second_val(second_value) {

}

ExtendedRange::ExtendedRange(const ExtendedRange& range): Range(range) {
    m_second_val = range.m_second_val;
}

/// PUBLIC SET
ExtendedRange& ExtendedRange::include_end(bool solution) {
    Range::include_end(solution);
    return (*this);
}

/// PUBLIC GET
string ExtendedRange::to_string(const t_str_cb& cb) const {
    std::string str;

    str.append("<");

    auto end = this->get_real_end();
    for(size_t i = m_start; i <= end; i++) {
        auto iter = std::to_string(i);
        auto second_val = std::to_string(m_second_val);

        str.append(cb(iter, second_val));
    }

    str.append(">");
    return str;
}

string ExtendedRange::to_string(bool order) const {
    std::string str;

    str.append("<");
    if(order) {
        str
         .append(std::to_string(m_second_val))
         .append(",")
         .append(std::to_string(m_start))
         .append("-")
         .append(std::to_string(this->get_real_end()));
    } else {
        str
         .append(std::to_string(m_start))
         .append("-")
         .append(std::to_string(this->get_real_end()))
         .append(",")
         .append(std::to_string(m_second_val));
    }
    str.append(">");

    return str;
}
