#include "RedisPool.h"

#include <glog/logging.h>
#include <string.h>

#include <type_traits>

RedisConn::RedisConn(const RedisInfo &info) : info_(info) {}

RedisConn::~RedisConn() { Release(); }

int RedisConn::Init() {
    if (context_) {
        return 0;
    }

    // 1s 尝试重连一次
    uint64_t cur_time = (uint64_t) time(NULL);
    if (cur_time < last_connect_time_ + 1) {
        // 重连尝试 间隔1秒
        LOG(WARNING) << "cur_time:" << cur_time
                     << "m_last_connect_time:" << last_connect_time_;
        return 1;
    }
    last_connect_time_ = cur_time;

    // 1000ms超时
    // 建立连接后使用 redisContext 来保存连接状态。
    // redisContext 在每次操作后会修改其中的 err 和  errstr
    // 字段来表示发生的错误码（大于0）和对应的描述。
    struct timeval timeout = {0, 1000000};
    context_ = redisConnectWithTimeout(info_.host.c_str(), info_.port, timeout);

    if (!context_ || context_->err) {
        LOG(ERROR) << "redisConnect failed, host: " << info_.host
                   << "port: " << info_.port;
        Release();
        return 1;
    }

    // 验证密码
    if (!info_.passwd.empty()) {
        bool ret = ExecRedisCommand("AUTH %s", info_.passwd.c_str());
        if (!ret) {
            LOG(ERROR) << "Authentication failure";
            return -1;
        }
    }

    bool ret = ExecRedisCommand("select %d", info_.db);

    if (!ret) {
        LOG(ERROR) << "select cache db failed";
        return 2;
    }

    return 0;
}

void RedisConn::Release() {
    if (context_) {
        redisFree(context_);
        context_ = nullptr;
    }
}

std::string RedisConn::Get(const std::string &key) {
    std::string result;
    ExecRedisCommand(result, "GET %s", key.c_str());
    return result;
}

std::string RedisConn::Set(const std::string &key, const std::string &value) {
    std::string result;
    ExecRedisCommand(result, "SET %s %s", key.c_str(), value.c_str());
    return result;
}

std::string RedisConn::SetEx(const std::string &key, const std::string &value,
                             int timeout) {
    std::string result;
    ExecRedisCommand(result, "SETEX %s %d %s", key.c_str(), timeout,
                     value.c_str());

    return result;
}

bool RedisConn::MGet(const std::vector<std::string> &keys,
                     std::map<std::string, std::string> &values) {
    if (keys.empty()) {
        return false;
    }

    std::string str_key;
    bool first = true;
    for (auto ite = keys.begin(); ite != keys.end(); ++ite) {
        if (first) {
            first = false;
            str_key = *ite;
        } else {
            str_key += " " + *ite;
        }
    }

    if (str_key.empty()) {
        return false;
    }
    return ExecRedisCommand(keys, values, str_key.c_str());
}

bool RedisConn::Incr(const std::string &key, int64_t &value) {
    return ExecRedisCommand(value, "INCR %s", key.c_str());
}

bool RedisConn::Decr(const std::string &key, int64_t &value) {
    return ExecRedisCommand(value, "DECR", key.c_str());
}

int64_t RedisConn::LPush(const std::string &key, const std::string &value) {
    int64_t result = -1;
    ExecRedisCommand(result, "LPUSH %s %s", key.c_str(), value.c_str());
    return result;
}

int64_t RedisConn::RPush(const std::string &key, const std::string &value) {
    int64_t result = -1;
    ExecRedisCommand(result, "RPUSH %s %s", key.c_str(), value.c_str());
    return result;
}

int64_t RedisConn::LLen(const std::string &key) {
    int64_t result = -1;
    ExecRedisCommand(result, "LLEN %s", key.c_str());
    return result;
}

bool RedisConn::LRange(const std::string &key, int64_t start, int64_t end,
                       std::list<std::string> &result) {
    return ExecRedisCommand(result, "LRANGE %s %d %d", key.c_str(), start, end);
}

template<typename... Args>
bool RedisConn::ExecRedisCommand(const char *format, Args... args) {
    redisReply *reply = (redisReply *) redisCommand(context_, format, args...);
    if (!reply || reply->type != REDIS_REPLY_STATUS) {
        if (reply) {
            freeReplyObject(reply);
        }
        return false;
    }
    freeReplyObject(reply);
    return true;
}

template<typename... Args>
bool RedisConn::ExecRedisCommand(int64_t &result, const char *format,
                                 Args... args) {
    result = 0;
    if (Init() != 0) {
        return false;
    }

    // 返回的结果存放在redisReply
    redisReply *reply = (redisReply *) redisCommand(context_, format, args...);

    if (!reply) {
        LOG(ERROR) << "redisCommand failed: " << context_->errstr;
        Release();
        return false;
    }
    result = reply->integer;
    freeReplyObject(reply);  // 释放资源

    return true;
}

template<typename... Args>
bool RedisConn::ExecRedisCommand(std::string &result, const char *format,
                                 Args... args) {
    if (Init() != 0) {
        return false;
    }

    // 返回的结果存放在redisReply
    redisReply *reply = (redisReply *) redisCommand(context_, format, args...);

    if (!reply) {
        LOG(ERROR) << "redisCommand failed: " << context_->errstr;
        Release();
        return false;
    }
    result.append(reply->str, reply->len);
    freeReplyObject(reply);  // 释放资源

    return true;
}

template<typename... Args>
bool RedisConn::ExecRedisCommand(std::list<std::string> &result,
                                 const char *format, Args... args) {
    if (Init() != 0) {
        return false;
    }

    // 返回的结果存放在redisReply
    redisReply *reply = (redisReply *) redisCommand(context_, format, args...);

    if (!reply) {
        LOG(ERROR) << "redisCommand failed: " << context_->errstr;
        Release();
        return false;
    }

    if (reply->type == REDIS_REPLY_ARRAY) {
        for (size_t i = 0; i < reply->elements; i++) {
            redisReply *value_reply = reply->element[i];
            result.push_back(std::string(value_reply->str, value_reply->len));
        }
    }
    freeReplyObject(reply);  // 释放资源

    return true;
}

template<typename... Args>
bool RedisConn::ExecRedisCommand(const std::vector<std::string> &keys,
                                 std::map<std::string, std::string> &result,
                                 const char *format, Args... args) {
    if (Init() != 0) {
        return false;
    }

    // 返回的结果存放在redisReply
    redisReply *reply = (redisReply *) redisCommand(context_, format, args...);

    if (!reply) {
        LOG(ERROR) << "redisCommand failed: " << context_->errstr;
        Release();
        return false;
    }
    if (reply->type == REDIS_REPLY_ARRAY) {
        for (size_t i = 0; i < reply->elements; ++i) {
            redisReply *child_reply = reply->element[i];
            if (child_reply->type == REDIS_REPLY_STRING) {
                result[keys[i]] = child_reply->str;
            }
        }
    }
    freeReplyObject(reply);  // 释放资源

    return true;
}

RedisPool::RedisPool(const RedisInfo &info) : info_(info) {}

RedisPool::~RedisPool() {
    exiting_ = true;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        cond_var_.notify_all();  // 通知所有在等待的
    }

    {
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto ite = free_list_.begin(); ite != free_list_.end(); ++ite) {
            RedisConn *ptr = *ite;
            delete ptr;
        }
    }

    free_list_.clear();
    cur_conn_cnt_ = 0;
}

int RedisPool::Init() {
    for (int i = 0; i < cur_conn_cnt_; i++) {
        RedisConn *conn_ptr = new RedisConn(info_);
        if (conn_ptr->Init()) {
            delete conn_ptr;
            return -1;
        }

        std::unique_lock<std::mutex> lock(mutex_);
        free_list_.push_back(conn_ptr);
    }
    LOG(INFO) << "init redis pool free list size: " << free_list_.size();
    return 0;
}

RedisConn *RedisPool::GetRedisConn(uint64_t timeout_ms) {
    if (exiting_.load()) {
        LOG(INFO) << "redis exiting";
        return nullptr;
    }
    std::unique_lock<std::mutex> lock(mutex_);
    // 当没有连接可以用时
    if (free_list_.empty()) {
        // 连接数量是否达到最大的连接数量
        if (cur_conn_cnt_ >= info_.max_conn) {
            // 如果已经到达了，看看是否需要超时等待, timeout_ms = 0, 阻塞等待
            if (timeout_ms <= 0) {
                LOG(INFO) << "wait ms: " << timeout_ms;
                cond_var_.wait(lock,
                               [this] { return (!free_list_.empty()) || exiting_; });
            } else {
                // return如果返回 false，继续wait(或者超时),
                // 如果返回true退出wait 1.m_free_list不为空 2.超时退出
                // 3. m_abort_request被置为true，要释放整个连接池
                cond_var_.wait_for(lock, std::chrono::milliseconds(timeout_ms), [this] {
                    return (!free_list_.empty()) || exiting_;
                });
                // 带超时功能时还要判断是否为空
                if (free_list_.empty()) {
                    return NULL;
                }
            }
        } else {
            // 还没有到最大连接则创建连接
            RedisConn *conn_ptr = new RedisConn(info_);
            if (conn_ptr->Init()) {
                delete conn_ptr;
                return nullptr;
            } else {
                free_list_.push_back(conn_ptr);
                cur_conn_cnt_++;
            }
        }
    }
    RedisConn *ptr = free_list_.front();
    free_list_.pop_front();

    return ptr;
}

void RedisPool::RelRedisConn(RedisConn *conn) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto ite = free_list_.begin();
    for (; ite != free_list_.end(); ++ite) {
        if (*ite == conn) {
            break;
        }
    }

    if (ite == free_list_.end()) {
        free_list_.push_back(conn);
        cond_var_.notify_one();  // 通知取队列
    } else {
        LOG(ERROR) << "RelDBConn failed";  // 不再次回收连接
    }
}

int RedisManager::Init(const std::vector<RedisInfo> &rInfo) {
    infos_ = rInfo;

    if (infos_.empty()) {
        LOG(ERROR) << "redis instance empty";
        return -1;
    }

    for (const RedisInfo &info: infos_) {
        if (info.host.empty() || (info.port <= 0 || info.port > 65535) ||
            (info.db < 0 || info.db > 15) || info.max_conn == 0) {
            LOG(ERROR) << "init redis poll pragram error, host: " << info.host
                       << "port: " << info.port << "db: " << info.db << "maxConn"
                       << info.max_conn;
            return -2;
        }
        RedisPool *pool_ptr = new RedisPool(info);
        if (pool_ptr->Init() != 0) {
            LOG(ERROR) << "init redis pool failed";
            return -3;
        }
        pool_map_.insert(make_pair(info.name, pool_ptr));
    }

    return 0;
}

RedisConn *RedisManager::GetRedisConn(const std::string &pool_name) {
    auto ite = pool_map_.find(pool_name);
    if (ite == pool_map_.end()) {
        return nullptr;
    }

    return ite->second->GetRedisConn();
}

void RedisManager::RelRedisConn(RedisConn *conn) {
    if (!conn) {
        return;
    }

    auto ite = pool_map_.find(conn->GetPoolName());
    if (ite != pool_map_.end()) {
        return ite->second->RelRedisConn(conn);
    }
    return;
}
