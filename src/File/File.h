//
// Created by user on 11.12.2020.
//

#ifndef STUD_GAME_FILE_H
#define STUD_GAME_FILE_H

#include <iostream>
#include <fstream>
#include <functional>
#include <experimental/filesystem>
#include <utility>

#include "../Constants/Constants.h"
#include "../Exception/Exception.h"

using namespace std;
namespace fs = std::experimental::filesystem;

class File {
    protected:
        string m_file_path {};

    public:
        explicit File(string file_path) {
            m_file_path = File::resolve_path(Constants::directory_path, std::move(file_path));
        }

        virtual bool check_file_exists() {
            if(m_file_path.length() == 0) {
                return false;
            }

            return fs::exists(m_file_path);
        }

        virtual const string& get_file_path() {
            return m_file_path;
        }

        static string resolve_path(string left, string right) {
            int pos = 0;
            while (pos != -1) {
                pos = right.find("../");
                if(pos != -1) {
                    int remove_to = left.rfind('/');
                    if(remove_to != 0) {
                        left = left.substr(0, remove_to);
                    }

                    right = right.substr(pos + 3);

                    continue;
                }

                pos = right.find("./");
                if(pos != -1) {
                    right = right.substr(pos + 2);

                    continue;
                }
            }

            return (left + "/" + right);
        }
};


#endif //STUD_GAME_FILE_H
