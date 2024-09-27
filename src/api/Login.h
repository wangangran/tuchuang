#pragma once

#include <string>
#include <utility>
#include <ylt/struct_json/json_reader.h>
#include <ylt/struct_json/json_writer.h>

struct LoginReq {
    std::string user;
    std::string pwd;
};
YLT_REFL(LoginReq, user, pwd);

struct LoginResp {
    int code = 1;
    std::string token;
};
YLT_REFL(LoginResp, code, token);

class Login {
public:
    explicit Login(const std::string_view &body_str) : body_str_(body_str) { Parse(); }
    void DoLogin();
    std::string GetResult();
private:
    void Parse();
    [[nodiscard]] int VerifyUserPassword() const;
    [[nodiscard]] int SetToken();
    [[nodiscard]] int LoadMyFilesCountAndSharePictureCount() const;
private:
    std::string_view body_str_;
    LoginReq req_;
    LoginResp resp_;
};
