//
// Created by user on 10.12.2020.
//

#include "FileReader.h"

using namespace data_types;

string FileReader::read() const {
    string data;
    string line;

    if(!m_file.check_file_exists()) {
        throw Exception(Exception::FileNotExist);
    }

    std::ifstream in(m_file.get_file_path());

    if (in.is_open()) {
        while (getline(in, line)) {
            data += line;
        }
    } else {
        throw Exception(Exception::FileRead);
    }

    in.close();

    return data;
}

json FileReader::to_json() const {
    return json::parse(this->read());
}

xml FileReader::read_xml(const File &file) {
    if(!file.check_file_exists()) {
        throw Exception(Exception::FileNotExist);
    }

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(file.get_file_path().c_str());
    if (!result) {
        throw Exception(Exception::FileRead);
    }

    return doc;
}


