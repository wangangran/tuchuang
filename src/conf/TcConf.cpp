//
// Created by root on 9/18/24.
//

#include "TcConf.h"

#include <iostream>

TcConf::TcConf(const std::string &conf_path) : conf_path_(conf_path) {}


int TcConf::parse() {
    std::ifstream file(conf_path_);
    if (!file.is_open()) {
        return -1;
    }

    file.seekg(0, std::ios::end);
    std::streampos file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string file_buf;
    file_buf.resize(file_size);

    file.read(&file_buf[0], file_size);

    file.close();
    iguana::from_yaml(info_, file_buf);

    return 0;
}