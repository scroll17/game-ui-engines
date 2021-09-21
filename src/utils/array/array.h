//
// Created by user on 08.02.2021.
//

#ifndef STUD_GAME_ARRAY_H
#define STUD_GAME_ARRAY_H

//#include <glob.h>   // size_t
#include <utility>  // swap
#include <random>   // random_device, mt19937, uniform_int_distribution
#include <iostream>

namespace utils {
    namespace array {
        void show_array(int *arr, size_t size);

        int *bubble_sort(int *arr, size_t arr_size, size_t start = 0, size_t stop = 0);

        int *generate_arr(size_t, size_t);

        int gen_random(size_t start, size_t end);
    }
}

#endif //STUD_GAME_ARRAY_H
