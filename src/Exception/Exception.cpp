//
// Created by user on 11.12.2020.
//

#include "Exception.h"

static map<ExceptionType, string> EXCEPTION_DETAILS {
  { ExceptionType::FileRead, "Error with read file." }
};

Exception::Exception(ExceptionType type, const string &message): m_error_type(type), m_custom_error_message(message) {

}

const ExceptionType &Exception::get_error_type() {
    return m_error_type;
}

const string &Exception::get_error_message() {
    if(m_custom_error_message.length() > 0) {
        return m_custom_error_message;
    } else {
        return EXCEPTION_DETAILS[m_error_type];
    }
}


