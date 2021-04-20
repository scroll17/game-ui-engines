//
// Created by user on 11.01.2021.
//

#ifndef STUD_GAME_TIME_H
#define STUD_GAME_TIME_H

#include <chrono> // chrono

namespace data_types {

    class Timer {
        private:
            using clock_t = std::chrono::high_resolution_clock;
            using second_t = std::chrono::duration<double, std::ratio<1> >;

            std::chrono::time_point<clock_t> m_beg;

        public:
            Timer(): m_beg(clock_t::now()) {};

            void reset();

            double elapsed() const;
    };

}


#endif //STUD_GAME_TIME_H
