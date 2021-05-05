//
// Created by user on 11.12.2020.
//

#ifndef STUD_GAME_EXCEPTION_H
#define STUD_GAME_EXCEPTION_H

#include "iostream"
#include <map>
#include <utility>
#include <SFML/Graphics.hpp>

using namespace std;

namespace data_types {

    class Exception final: public std::exception {
        public:
            enum Type {
                FileRead,
                FileOpen,
                FileNotExist,
                FileNotOpen,
                ElementNotBuild,
                NonExistentPosition,
                RequiredVariableMissing,
                ActionBeforeRequired,
                InvalidArgument,
                UnknownArgument,
                CannotRender
            };

            explicit Exception(Type type, string message = "");
            ~Exception() = default;

            const Type& get_error_type() const;
            const string& get_error_message() const;

            void draw_error() const;

            virtual const char* what() const noexcept override final;

        private:
            Type m_error_type;
            const string m_custom_error_message;
    };

}


#endif //STUD_GAME_EXCEPTION_H
