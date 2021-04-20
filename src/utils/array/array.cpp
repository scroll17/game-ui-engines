//
// Created by user on 08.02.2021.
//

#include "array.h"

int *utils::array::bubble_sort(int *arr, size_t arr_size, size_t start, size_t stop) {
    if(stop == 0) {
        stop = arr_size;
    }

    size_t sort_to = stop;
    while (sort_to > 1) {
        size_t curr_index = start + 1;

        while (curr_index < sort_to) {
            size_t prev_index = curr_index - 1;

            if(arr[prev_index] > arr[curr_index]) {
                std::swap(arr[prev_index], arr[curr_index]);
            }

            curr_index++;
        }

        sort_to -= 1;
    }

    return arr;
}

void utils::array::show_array(int *arr, size_t arr_size) {
    std::cout << "[";

    for(size_t i = 0; i < arr_size; i++) {
        std::cout << arr[i];

        if(i < (arr_size - 1)) {
            std::cout << ",";
        }
    }

    std::cout << "]";
}

int utils::array::gen_random(size_t start, size_t end) {
    std::random_device rd;
    std::mt19937 mt(rd());

    std::uniform_int_distribution<int> distribution(start, end);

    return distribution(mt);
}

int *utils::array::generate_arr(size_t size, size_t range) {
    int *arr = new int[size];

    for(int i = 0; i < size; i++) {
        int val = utils::array::gen_random(0, range);

        arr[i] = val;
    }

    return arr;
}