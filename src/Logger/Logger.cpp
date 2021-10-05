//
// Created by user on 05.10.21.
//

#include "Logger.h"

// PUBLIC GET
std::ostream& Logger::log() {
    last_log_key = "";
    return std::cout << this->get_log_type_str() << ": ";
}

// PUBLIC SET
std::ostream& Logger::log_uniq(std::string key, std::string val) {
    if(key != last_log_key) {
        last_log_key = key;
        return std::cout
            << this->get_log_type_str()
            << ": "
            << key
            << val
            << std::endl;
    }

    return std::cout << "";
}


// PRIVATE GET
std::string Logger::get_log_type_str() const {
    std::string label;

    switch(log_type) {
        case DEBUG: label = "DEBUG"; break;
        case INFO:  label = "INFO "; break;
        case WARN:  label = "WARN "; break;
        case ERROR: label = "ERROR"; break;
    }

    return label;
}