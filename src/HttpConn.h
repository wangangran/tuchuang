#pragma once

#include <memory>
#include <ylt/coro_io/coro_io.hpp>
#include <ylt/coro_http/coro_http_server.hpp>

struct TcConf;

class HttpConn {
public:
    explicit HttpConn(const std::shared_ptr<TcConf> &info) : tc_info_(info) {}

    async_simple::coro::Lazy<void> Run();
// private:
    // 账号注册
    static int HandleRegisterRequest(cinatra::coro_http_request &req, cinatra::coro_http_response &resp);
    // 账号登录
    static int HandleLoginRequest(cinatra::coro_http_request &req, cinatra::coro_http_response &resp);
private:
    std::shared_ptr<TcConf> tc_info_;
};