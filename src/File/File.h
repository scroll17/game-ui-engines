//
// Created by user on 11.12.2020.
//

#ifndef STUD_GAME_FILE_H
#define STUD_GAME_FILE_H

#include <fstream>
#include <utility>

#include "pugixml.hpp"
#include <nlohmann/json.hpp>

#include "../Constants/Constants.h"
#include "../Exception/Exception.h"

using namespace std;

using json = nlohmann::json;
using xml = pugi::xml_document;

class File {
    protected:
        string m_file_path;

        virtual json to_json() const;
        virtual xml to_xml() const;

    public:
        explicit File(string file_path);
        ~File() = default;

        bool check_file_exists() const;
        const string& get_file_path() const;

        static string resolve_path(string left, string right);
};

#endif //STUD_GAME_FILE_H
