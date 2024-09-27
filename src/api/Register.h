#pragma once

#include <string_view>
#include <ylt/struct_json/json_reader.h>
#include <ylt/struct_json/json_writer.h>

struct RegisterInfo {
    std::string userName;
    std::string nickName;
    std::string firstPwd;
    std::string phone;
    std::string email;
};
YLT_REFL(RegisterInfo, userName, nickName, firstPwd, phone, email);

struct RegisterResult {
    int code;
};
YLT_REFL(RegisterResult, code);

class Register {
public:
    explicit Register(const std::string_view &body_str) : body_str_(body_str) { Parse(); }
    std::string RegUser();
private:
    void Parse();
private:
    std::string_view body_str_;
    RegisterInfo info_;
};