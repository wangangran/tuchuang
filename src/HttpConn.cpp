#include "HttpConn.h"

#include "conf/TcConf.h"
#include "api/Register.h"
#include "api/Login.h"

using namespace cinatra;

async_simple::coro::Lazy<void> HttpConn::Run() {
    coro_http_server server(1, tc_info_->info_.http_port);
    server.set_http_handler<POST>(
            "/api/reg",
            &HttpConn::HandleRegisterRequest);

    server.set_http_handler<POST>(
            "/api/login",
            &HttpConn::HandleLoginRequest);

    server.sync_start();

    co_return;
}

int HttpConn::HandleRegisterRequest(cinatra::coro_http_request &req, cinatra::coro_http_response &resp) {
    Register reg(req.get_body());
    resp.set_status_and_content(status_type::ok, reg.RegUser());
    return 0;
}

int HttpConn::HandleLoginRequest(cinatra::coro_http_request &req, cinatra::coro_http_response &resp) {
    Login login_obj(req.get_body());
    login_obj.DoLogin();
    resp.set_status_and_content(status_type::ok, login_obj.GetResult());
    return 0;
}