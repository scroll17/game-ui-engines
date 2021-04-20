//
// Created by user on 11.12.2020.
//

#include "FileWriter.h"

using namespace data_types;

FileWriter::~FileWriter() {
    this->close_file();
}

FileWriter& FileWriter::open_file() {
    if(!m_file.check_file_exists()) {
        throw Exception(Exception::FileNotExist);
    }

    if(m_out.is_open()) return (*this);

    m_out.open(m_file.get_file_path());

    if(!m_out.is_open()) {
        throw Exception(Exception::FileOpen);
    }

    return (*this);
}

FileWriter& FileWriter::close_file() {
    if(m_out.is_open()) {
        m_out.close();
    }

    return (*this);
}

FileWriter& FileWriter::write(const string& data) {
    if(!m_out.is_open()) throw Exception(Exception::FileNotOpen);

    m_out << data.c_str();

    return (*this);
}

FileWriter& FileWriter::from_json(const json& data) {
    if(!m_out.is_open()) throw Exception(Exception::FileNotOpen);

    m_out << data;

    return (*this);
}

void FileWriter::save_xml(const File& file, const xml& data) {
    data.save_file(file.get_file_path().c_str());
}

