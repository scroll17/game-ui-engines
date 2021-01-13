//
// Created by user on 11.01.2021.
//

#ifndef STUD_GAME_STRING_H
#define STUD_GAME_STRING_H

// 1.5 times slower
int find(const char *str, const char* sub_str);

// 400 times faster
int rfind(const char *str, const char* sub_str);

#endif //STUD_GAME_STRING_H
