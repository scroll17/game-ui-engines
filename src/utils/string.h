//
// Created by user on 11.01.2021.
//

#ifndef STUD_GAME_STRING_H
#define STUD_GAME_STRING_H

#include <cstring>  // strlen

#include <locale>   // wstring_convert
#include <codecvt>  // codecvt_utf8_utf16
#include <string>   // wstring

using namespace std;

// 1.5 times slower
int find(const char *str, const char* sub_str);

// 400 times faster
int rfind(const char *str, const char* sub_str);

std::wstring convert_to_ws(const string& str);

#endif //STUD_GAME_STRING_H
