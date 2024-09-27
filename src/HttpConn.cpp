#include "HttpConn.h"

#include <functional>

#include "conf/TcConf.h"
#include "api/Register.h"

using namespace cinatra;

struct log_t {
    bool before(coro_http_request &, coro_http_response &) {
        std::cout << "before log" << std::endl;
        return true;
    }

    bool after(coro_http_request &, coro_http_response &res) {
        std::cout << "after log" << std::endl;
        res.add_header("aaaa", "bbcc");
        return true;
    }
};

struct get_data {
    bool before(coro_http_request &req, coro_http_response &res) {
        req.set_aspect_data("hello world");
        return true;
    }
};

async_simple::coro::Lazy<void> HttpConn::Run() {
    coro_http_server server(1, tc_info_->info_.http_port);
    server.set_http_handler<POST>(
            "/api/reg",
            &HttpConn::HandleRegisterRequest,
            *this,
            log_t{},
            get_data{});

    server.sync_start();

    co_return;
}

void HttpConn::HandleRegisterRequest(cinatra::coro_http_request &req, cinatra::coro_http_response &resp) {
    Register reg(req.get_body());
    resp.set_status_and_content(status_type::ok, reg.RegUser());
}