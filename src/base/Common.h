#pragma once

#include <string>
#include <memory>

class Common {
public:
    template<typename... Args>
    static std::string formatString(const std::string &format, Args... args) {
        auto size = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;
        std::unique_ptr<char[]> buf(new char[size]);
        std::snprintf(buf.get(), size, format.c_str(), args...);
        return std::string(buf.get(), buf.get() + size - 1);
    }

    static std::string RandomString(const int len) {
        std::string str;
        for (int idx = 0; idx < len; idx++) {
            str.push_back('a' + rand() % 26);
        }
        return str;
    }

};