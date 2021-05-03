//
// Created by user on 30.04.2021.
//

#ifndef STUD_GAME_EXTENDEDRANGE_H
#define STUD_GAME_EXTENDEDRANGE_H

#include <string>
#include <functional>

#include "../Range/Range.h"

using namespace std;

namespace data_types {

    class ExtendedRange: public Range {
        public:
            using t_str_cb = function<string(const string& iter, const string& second_v)>;

        protected:
            size_t m_second_val;

        public:
            ExtendedRange(size_t start, size_t end, size_t second_value);
            ExtendedRange(const ExtendedRange&);
            virtual ~ExtendedRange() = default;

            ExtendedRange& include_end(bool solution) override;

            [[nodiscard]] string to_string(const t_str_cb& cb) const;
            [[nodiscard]] string to_string(bool order) const;
    };

}

#endif //STUD_GAME_EXTENDEDRANGE_H
