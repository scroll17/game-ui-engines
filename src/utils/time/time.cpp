//
// Created by user on 11.04.2021.
//

#include "./time.h"

using namespace std;

long utils::time::get_timestamp() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}