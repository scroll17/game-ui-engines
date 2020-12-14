//
// Created by user on 11.12.2020.
//

#ifndef STUD_GAME_EXCEPTION_H
#define STUD_GAME_EXCEPTION_H

#include "iostream"
#include <map>

using namespace std;

enum class ExceptionType {
    FileRead,
    FileNotExist
};

class Exception {
    private:
        ExceptionType m_error_type;
        const string &m_custom_error_message;

    public:
        explicit Exception(ExceptionType type, const string &message = "");
        ~Exception() = default;

        const ExceptionType& get_error_type();
        const string& get_error_message();
};

#endif //STUD_GAME_EXCEPTION_H
