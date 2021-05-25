//
// Created by user on 05.05.2021.
//

#include <iostream>
#include <iomanip>
#include <algorithm>

#include "../DataTypes/Time/Time.h"
#include "../utils/array/array.h"

using namespace std;

void show_diff(double v_1, double v_2) {
    double diff = v_2 - v_1;

    if(diff < 0) {
        cout << "Алгоритм #1 медленее #2 на "
             << std::fixed << std::setprecision(12)
             << std::abs(diff)
             << endl;
    } else {
        cout << "Алгоритм #1 быстрее #2 на "
             << std::fixed << std::setprecision(12)
             << diff
             << endl;
    }
}

void bubble_sort_algorithm_test() {
    double t_v_1, t_v_2;

    int array_size = 10000;
    int *arr_1 = utils::array::generate_arr(array_size, 1000);
    int *arr_2 = utils::array::generate_arr(array_size, 1000);

    data_types::Timer t1;

    utils::array::bubble_sort(arr_1, array_size);
    t_v_1 = t1.elapsed();

    t1.reset();

    std::sort(arr_2, arr_2 + array_size);
    t_v_2 = t1.elapsed();

    std::cout << "#1: " << std::fixed << std::setprecision(12) << t_v_1 << endl;
    std::cout << "#2: " << std::fixed << std::setprecision(12) << t_v_2 << endl;

    show_diff(t_v_1, t_v_2);

    delete[] arr_1;
    delete[] arr_2;
}

int main () {
    bubble_sort_algorithm_test();
    return EXIT_SUCCESS;
}