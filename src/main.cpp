#include <glog/logging.h>

#include <iostream>
#include <chrono>
#include <ylt/coro_http/coro_http_client.hpp>
#include <ylt/coro_http/coro_http_server.hpp>
#include <ylt/coro_io/coro_io.hpp>

#include "conf/TcConf.h"
#include "db/mysql/MysqlPool.h"
#include "db/redis/RedisPool.h"
#include "HttpConn.h"

using namespace cinatra;
using namespace std::chrono_literals;

static const std::string gLogFile = "/srv/tuchuang/log/";

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "args error, program exit" << std::endl;
        return -1;
    }

    std::shared_ptr<TcConf> conf(new TcConf(argv[1]));
    if (conf->parse() != 0) {
        std::cout << "parse tc conf error" << std::endl;
        return -1;
    }

    google::InitGoogleLogging(argv[0]);
    // 设置日志输出目录
    // FLAGS_log_dir = conf->info_.log_dir;

    LOG(INFO) << "This is an info message written to a file.";

    if (MysqlManagerIns()->Init(conf->info_.mysql_info) != 0) {
        LOG(ERROR) << "mysql init failed";
        return -1;
    }

    if (RedisManagerIns->Init(conf->info_.redis_info) != 0) {
        LOG(ERROR) << "redis init failed";
        return -1;
    }

    RedisConn *conn = RedisManagerIns->GetRedisConn("token");
    if (conn == nullptr) {
        LOG(ERROR) << "get redis conn error";
    }

    conn->Set("name", "war");
    LOG(INFO) << conn->Get("name");


    HttpConn http(conf);
    async_simple::coro::syncAwait(http.Run());
    while (true) {
        std::this_thread::sleep_for(500ms);
    }
    google::ShutdownGoogleLogging();
    return 0;
}
