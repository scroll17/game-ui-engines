//
// Created by user on 10.12.2020.
//

#ifndef STUD_GAME_FILEREADER_H
#define STUD_GAME_FILEREADER_H

#include "pugixml.hpp"
#include <nlohmann/json.hpp>

#include "../File.h"

using json = nlohmann::json;
using xml = pugi::xml_document;

class FileReader {
    private:
        File m_file;

    public:
        FileReader() = delete;

        explicit FileReader(const string& file_path): m_file(file_path) {};
        ~FileReader() = default;

        [[nodiscard]] string read() const;
        [[nodiscard]] json to_json() const;

        static xml read_xml(const File &file);
};


#endif //STUD_GAME_FILEREADER_H
