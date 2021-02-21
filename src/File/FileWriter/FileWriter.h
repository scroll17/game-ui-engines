//
// Created by user on 11.12.2020.
//

#ifndef STUD_GAME_FILEWRITER_H
#define STUD_GAME_FILEWRITER_H

#include "pugixml.hpp"
#include <nlohmann/json.hpp>

#include "../File.h"

using json = nlohmann::json;
using xml = pugi::xml_document;

class FileWriter {
    private:
        File m_file;
        ofstream m_out;

    public:
        FileWriter() = delete;

        explicit FileWriter(const string& file_path): m_file(file_path) {};
        ~FileWriter();

        FileWriter& open_file();
        FileWriter& close_file();

        FileWriter& write(const string& data);
        FileWriter& from_json(const json& data);

        static void save_xml(const File& file, const xml& data);
};

#endif //STUD_GAME_FILEWRITER_H