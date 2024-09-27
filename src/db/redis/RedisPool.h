#pragma once

#include <hiredis/hiredis.h>

#include <any>
#include <condition_variable>
#include <list>
#include <map>
#include <mutex>
#include <vector>

#include "base/Singleton.h"
#include "conf/TcConf.h"

class RedisConn {
public:
    explicit RedisConn(const RedisInfo &info);

    ~RedisConn();

    int Init();

    void Release();

    std::string GetPoolName() const { return info_.name; }

    // ==========string相关操作==========
    std::string Get(const std::string &key);

    std::string Set(const std::string &key, const std::string &value);

    std::string SetEx(const std::string &key, const std::string &value,
                      int timeout = 0);

    bool MGet(const std::vector<std::string> &keys,
              std::map<std::string, std::string> &values);

    bool Incr(const std::string &key, int64_t &value);

    bool Decr(const std::string &key, int64_t &value);

    // ==========list相关操作==========
    int64_t LPush(const std::string &key, const std::string &value);

    int64_t RPush(const std::string &key, const std::string &value);

    int64_t LLen(const std::string &key);

    bool LRange(const std::string &key, int64_t start, int64_t end,
                std::list<std::string> &result);

    // ==========zset相关操作==========
private:
    template<typename... Args>
    bool ExecRedisCommand(const char *format, Args... args);

    template<typename... Args>
    bool ExecRedisCommand(int64_t &result, const char *format, Args... args);

    template<typename... Args>
    bool ExecRedisCommand(std::string &result, const char *format, Args... args);

    template<typename... Args>
    bool ExecRedisCommand(std::list<std::string> &result, const char *format, Args... args);

    template<typename... Args>
    bool ExecRedisCommand(const std::vector<std::string> &keys,
                          std::map<std::string, std::string> &result,
                          const char *format, Args... args);

private:
    redisContext *context_ = nullptr;  // redis上下文
    uint64_t last_connect_time_ = 0;
    RedisInfo info_;
};

class RedisPool {
public:
    explicit RedisPool(const RedisInfo &info);

    ~RedisPool();

    int Init();

    RedisConn *GetRedisConn(uint64_t timeout_ms = 0);

    void RelRedisConn(RedisConn *conn);

private:
    std::atomic<bool> exiting_ = false;
    std::mutex mutex_;
    std::condition_variable cond_var_;
    RedisInfo info_;
    int cur_conn_cnt_ = 2;
    std::list<RedisConn *> free_list_;
};

class RedisManager {
public:
    int Init(const std::vector<RedisInfo> &rInfo);

    RedisConn *GetRedisConn(const std::string &pool_name);

    void RelRedisConn(RedisConn *conn);

private:
    std::string cacheInstances_;
    std::vector<RedisInfo> infos_;
    std::map<std::string, RedisPool *> pool_map_;
};

#define RedisManagerIns Singleton<RedisManager>::instance

class AutoRelRedisConn {
public:
    explicit AutoRelRedisConn(RedisConn *conn) : conn_(conn) {}
    ~AutoRelRedisConn() {
        RedisManagerIns()->RelRedisConn(conn_);
    }
private:
    RedisConn *conn_ = nullptr;
};
