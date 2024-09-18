//
// Created by root on 9/18/24.
//

#include "TcConf.h"

#include <iostream>

TcConf::TcConf(const std::string &confPath) : confPath_(confPath) {}


int TcConf::parse() {
    std::ifstream file(confPath_);
    if (!file.is_open()) {
        return -1;
    }

    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string fileBuf;
    fileBuf.resize(fileSize);

    file.read(&fileBuf[0], fileSize);

    file.close();
    std::cout << fileBuf << std::endl;
    TcConfInfo tc;
    iguana::from_yaml(tc, fileBuf);

    std::cout << tc.sqlMaster.host << std::endl;
    return 0;
}