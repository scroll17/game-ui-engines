//
// Created by user on 11.01.2021.
//

#include "./string.h"

static int find_stop_symbol_pos(const char *str, const int str_len, int curr_pos, const char sym) {
    for(; curr_pos < str_len; curr_pos++) {
        if(str[curr_pos] == sym) return curr_pos;
    }

    return -1;
}

static int rfind_stop_symbol_pos(const char *str, int curr_pos, const char sym) {
    while(curr_pos >= 0) {
        if(str[curr_pos] == sym) return curr_pos;
        curr_pos--;
    }

    return -1;
}

static int find_last_eq_pos(const char *str, int pos_str, const char *sub_str, const int sub_str_len) {
    int sub_str_pos = 0;

    if(str[pos_str] != sub_str[sub_str_pos]) {
        return -1;
    }

    int last_eq = 0;
    while (sub_str_pos < sub_str_len) {
        if(str[pos_str] == sub_str[sub_str_pos]) {
            last_eq = pos_str;

            pos_str++;
            sub_str_pos++;
        } else {
            break;
        }
    }

    return last_eq;
}

extern int find(const char *str, const char* sub_str) {
    bool is_empty_strings = (int(str[0]) == 0) || (int(sub_str[0]) == 0);
    if(is_empty_strings) {
        return -1;
    }

    const int str_len = strlen(str);
    const int sub_str_len = strlen(sub_str);

    if(sub_str_len > str_len) {
        return -1;
    }

    int pos = find_stop_symbol_pos(str, str_len, 0, sub_str[0]);
    if(pos == -1) {
        return -1;
    }

    while (true) {
        int prev_pos = pos;

        pos = find_last_eq_pos(str, pos, sub_str, sub_str_len);
        if(pos == -1) break;

        int diff_pos = pos - prev_pos;
        if(diff_pos == (sub_str_len - 1)) {
            return prev_pos;
        } else {
            pos = find_stop_symbol_pos(str, str_len, pos + sub_str_len, sub_str[0]);
            if(pos == -1) break;

            if(pos >= str_len) {
                return -1;
            }
        }
    }

    return -1;
}

extern int rfind(const char *str, const char* sub_str) {
    bool is_empty_strings = (int(str[0]) == 0) || (int(sub_str[0]) == 0);
    if(is_empty_strings) {
        return -1;
    }

    const int str_len = strlen(str);
    const int sub_str_len = strlen(sub_str);

    if(sub_str_len > str_len) {
        return -1;
    }

    int pos = rfind_stop_symbol_pos(str, str_len - 1, sub_str[0]);
    if(pos == -1) {
        return -1;
    }

    while (true) {
        int prev_pos = pos;

        pos = find_last_eq_pos(str, pos, sub_str, sub_str_len);
        if(pos == -1) break;

        int diff_pos = pos - prev_pos;
        if(diff_pos == (sub_str_len - 1)) {
            return prev_pos;
        } else {
            pos = rfind_stop_symbol_pos(str, prev_pos - (sub_str_len - 1), sub_str[0]);

            if(pos < 0) {
                return -1;
            }
        }
    }

    return -1;
}

std::wstring convert_to_ws(const string& str) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wide = converter.from_bytes(str);

    return wide;
}