//
// Created by user on 17.04.2021.
//

#ifndef STUD_GAME_RANGE_H
#define STUD_GAME_RANGE_H

#include "iostream"
#include <functional>

namespace data_types {

    class Range {
            using t_cb = std::function<void(size_t)>;

        private:
            size_t m_start;
            size_t m_end;

            bool m_include_end;

        public:
            Range(size_t start, size_t end, bool include_end = false);
            ~Range() = default;

            void include_end(bool solution);
            void execute(const t_cb& cb) const;

            [[nodiscard]] bool in_range(size_t val) const;
            [[nodiscard]] size_t get_start() const;
            [[nodiscard]] size_t get_end() const;

            static void for_each(size_t start, size_t end, const t_cb& cb);
            static void for_each(const Range& range, const t_cb& cb);
    };

}

#endif //STUD_GAME_RANGE_H
