//
// Created by user on 05.10.21.
//

#ifndef STUD_GAME_LOGGER_H
#define STUD_GAME_LOGGER_H

#include "iostream"

class Logger {
    public:
        enum typelog {
            DEBUG,
            INFO,
            WARN,
            ERROR
        };

    private:
        typelog log_type;
        std::string last_log_key;

        std::string get_log_type_str() const;

    public:
        Logger(typelog type): log_type(type) {};
        ~Logger() = default;

        std::ostream& log();
        std::ostream& log_uniq(std::string key, std::string val);
};


#endif //STUD_GAME_LOGGER_H
