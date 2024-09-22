#include <glog/logging.h>

#include <iostream>

#include "conf/TcConf.h"
#include "db/redis/RedisPool.h"

static const std::string gLogFile = "/srv/tuchuang/log/";

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cout << "args error, program exit" << std::endl;
    return -1;
  }

  TcConf conf(argv[1]);
  if (conf.parse() != 0) {
    std::cout << "parse tc conf error" << std::endl;
    return -1;
  }

  google::InitGoogleLogging(argv[0]);
  // 设置日志输出目录
  FLAGS_log_dir = conf.info_.log_dir;

  LOG(INFO) << "This is an info message written to a file.";

  if (RedisManagerIns->Init(conf.info_.redis_info) != 0) {
    LOG(ERROR) << "redis init failed";
    return -1;
  }

  RedisConn *conn = RedisManagerIns->GetRedisConn("token");
  if (conn == nullptr) {
    LOG(ERROR) << "get redis conn error";
  }

  conn->Set("name", "war");
  LOG(INFO) << conn->Get("name");

  google::ShutdownGoogleLogging();
  return 0;
}
