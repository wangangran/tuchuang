#include "MysqlPool.h"

#include <glog/logging.h>
#include <cstring>

#include <utility>

MysqlResultSet::MysqlResultSet(MYSQL_RES *res) {
    res_ = res;
    // map table field key to index in the result array
    uint32_t num_fields = mysql_num_fields(res_); // 返回结果集中的行数。
    MYSQL_FIELD *fields = mysql_fetch_fields(res_); // 关于结果集所有列的MYSQL_FIELD结构的数组
    for (uint32_t i = 0; i < num_fields; i++) {
        key_map_.insert(std::make_pair(fields[i].name, i)); // 每个结构提供了结果集中1列的字段定义
        LOG(INFO) << "num_fields fields[" << i << "].name: " << fields[i].name;
    }
}

MysqlResultSet::~MysqlResultSet() {
    if (res_) {
        mysql_free_result(res_);
        res_ = nullptr;
    }
}

bool MysqlResultSet::Next() {
    row_ = mysql_fetch_row(res_); // 检索结果集的下一行,行内值的数目由mysql_num_fields(result)给出
    if (row_) {
        return true;
    } else {
        return false;
    }
}

int MysqlResultSet::GetIndex(const char *key) {
    auto ite = key_map_.find(key);
    if (ite == key_map_.end()) {
        return -1;
    }
    return ite->second;
}

int MysqlResultSet::GetInt(const char *key) {
    int idx = GetIndex(key); // 查找列的索引
    if (idx == -1) {
        return 0;
    }
    return atoi(row_[idx]); // 有索引
}

char *MysqlResultSet::GetString(const char *key) {
    int idx = GetIndex(key);
    if (idx == -1) {
        return nullptr;
    }
    return row_[idx]; // 列
}

MysqlPreStmt::MysqlPreStmt() {
    stmt_ = nullptr;
    param_bind_ = nullptr;
    param_cnt_ = 0;
}

MysqlPreStmt::~MysqlPreStmt() {
    if (stmt_) {
        mysql_stmt_close(stmt_);
        stmt_ = nullptr;
    }

    if (param_bind_) {
        delete[] param_bind_;
        param_bind_ = nullptr;
    }
}

bool MysqlPreStmt::Init(MYSQL *mysql, std::string &sql) {
    mysql_ping(mysql); // 当mysql连接丢失的时候，使用mysql_ping能够自动重连数据库

    // g_master_conn_fail_num ++;
    stmt_ = mysql_stmt_init(mysql);
    if (!stmt_) {
        LOG(ERROR) << "mysql_stmt_init failed";
        return false;
    }

    if (mysql_stmt_prepare(stmt_, sql.c_str(), sql.size())) {
        LOG(ERROR) << "mysql_stmt_prepare failed: " << mysql_stmt_error(stmt_);
        return false;
    }

    param_cnt_ = mysql_stmt_param_count(stmt_);
    if (param_cnt_ > 0) {
        param_bind_ = new MYSQL_BIND[param_cnt_];
        if (!param_bind_) {
            LOG(ERROR) << "new failed";
            return false;
        }

        memset(param_bind_, 0, sizeof(MYSQL_BIND) * param_cnt_);
    }

    return true;
}

void MysqlPreStmt::SetParam(uint32_t index, int &value) {
    if (index >= param_cnt_) {
        LOG(ERROR) << "index too large: " << index;
        return;
    }

    param_bind_[index].buffer_type = MYSQL_TYPE_LONG;
    param_bind_[index].buffer = &value;
}

void MysqlPreStmt::SetParam(uint32_t index, uint32_t &value) {
    if (index >= param_cnt_) {
        LOG(ERROR) << "index too large: " << index;
        return;
    }

    param_bind_[index].buffer_type = MYSQL_TYPE_LONG;
    param_bind_[index].buffer = &value;
}

void MysqlPreStmt::SetParam(uint32_t index, std::string &value) {
    if (index >= param_cnt_) {
        LOG(ERROR) << "index too large: " << index;
        return;
    }

    param_bind_[index].buffer_type = MYSQL_TYPE_STRING;
    param_bind_[index].buffer = (char *) value.c_str();
    param_bind_[index].buffer_length = value.size();
}

void MysqlPreStmt::SetParam(uint32_t index, const std::string &value) {
    if (index >= param_cnt_) {
        LOG(ERROR) << "index too large: " << index;
        return;
    }

    param_bind_[index].buffer_type = MYSQL_TYPE_STRING;
    param_bind_[index].buffer = (char *) value.c_str();
    param_bind_[index].buffer_length = value.size();
}

bool MysqlPreStmt::ExecuteUpdate() {
    if (!stmt_) {
        LOG(ERROR) << "no m_stmt";
        return false;
    }

    if (mysql_stmt_bind_param(stmt_, param_bind_)) {
        LOG(ERROR) << "mysql_stmt_bind_param failed: " << mysql_stmt_error(stmt_);
        return false;
    }

    if (mysql_stmt_execute(stmt_)) {
        LOG(ERROR) << "mysql_stmt_execute failed: " << mysql_stmt_error(stmt_);
        return false;
    }

    if (mysql_stmt_affected_rows(stmt_) == 0) {
        LOG(ERROR) << "ExecuteUpdate have no effect";
        return false;
    }

    return true;
}

uint32_t MysqlPreStmt::GetInsertId() {
    return mysql_stmt_insert_id(stmt_);
}

MysqlConn::MysqlConn(MysqlInfo info) : info_(std::move(info)) {}

MysqlConn::~MysqlConn() { Release(); }

int MysqlConn::Init() {
    mysql_ = mysql_init(nullptr); // mysql_标准的mysql c client对应的api
    if (!mysql_) {
        LOG(ERROR) << "mysql_init failed";
        return -1;
    }

    mysql_options(mysql_, MYSQL_SET_CHARSET_NAME,
                  "utf8mb4"); // utf8mb4和utf8区别

    // ip 端口 用户名 密码 数据库名
    if (!mysql_real_connect(mysql_, info_.host.c_str(), info_.user_name.c_str(), info_.passwd.c_str(),
                            info_.db_name.c_str(), info_.port, nullptr, 0)) {
        LOG(ERROR) << "mysql_real_connect failed: " << mysql_error(mysql_);
        return -2;
    }

    return 0;
}

void MysqlConn::Release() {
    if (mysql_) {
        mysql_close(mysql_);
        mysql_ = nullptr;
    }
}

bool MysqlConn::ExecuteCreate(const char *sql_query) {
    mysql_ping(mysql_);
    // mysql_real_query 实际就是执行了SQL
    if (mysql_real_query(mysql_, sql_query, strlen(sql_query))) {
        LOG(ERROR) << "mysql_real_query failed: " << mysql_error(mysql_);
        return false;
    }

    return true;
}

bool MysqlConn::ExecuteDrop(const char *sql_query) {
    mysql_ping(mysql_); // 如果端开了，能够自动重连

    if (mysql_real_query(mysql_, sql_query, strlen(sql_query))) {
        LOG(ERROR) << "mysql_real_query failed: " << mysql_error(mysql_);
        return false;
    }

    return true;
}

MysqlResultSet *MysqlConn::ExecuteQuery(const char *sql_query) {
    mysql_ping(mysql_);
    row_num_ = 0;
    if (mysql_real_query(mysql_, sql_query, strlen(sql_query))) {
        LOG(ERROR) << "mysql_real_query failed: " << mysql_error(mysql_) << "sql: " << sql_query;
        return nullptr;
    }

    MYSQL_RES *res = mysql_store_result(mysql_);
    if (!res) {
        LOG(ERROR) << "mysql_store_result failed: " << mysql_error(mysql_);
        return nullptr;
    }
    row_num_ = mysql_num_rows(res);

    return new MysqlResultSet(res);
}

bool MysqlConn::ExecutePassQuery(const char *sql_query) {
    mysql_ping(mysql_);
    if (mysql_real_query(mysql_, sql_query, strlen(sql_query))) {
        LOG(ERROR) << "mysql_real_query failed: " << mysql_error(mysql_);
        return false;
    }

    return true;
}

bool MysqlConn::ExecuteUpdate(const char *sql_query, bool care_affected_rows) {
    mysql_ping(mysql_);

    if (mysql_real_query(mysql_, sql_query, strlen(sql_query))) {
        LOG(ERROR) << "mysql_real_query failed: " << mysql_error(mysql_) << "sql: " << sql_query;
        return false;
    }

    if (mysql_affected_rows(mysql_) > 0) {
        return true;
    } else {
        if (care_affected_rows) { // 如果在意影响的行数时, 返回false,否则返回true
            LOG(ERROR) << "mysql_real_query failed: " << mysql_error(mysql_) << "sql: " << sql_query;
            return false;
        } else {
            LOG(WARNING) << "affected_rows=0, sql: " << sql_query;
            return true;
        }
    }
}

bool MysqlConn::StartTransaction() {
    mysql_ping(mysql_);

    if (mysql_real_query(mysql_, "start transaction\n", 17)) {
        LOG(ERROR) << "mysql_real_query failed: " << mysql_error(mysql_) << " start transaction failed";
        return false;
    }

    return true;
}

bool MysqlConn::Rollback() {
    mysql_ping(mysql_);

    if (mysql_real_query(mysql_, "rollback\n", 8)) {
        LOG(ERROR) << "mysql_real_query failed: " << mysql_error(mysql_) << " sql: rollback";
        return false;
    }

    return true;
}

bool MysqlConn::Commit() {
    mysql_ping(mysql_);

    if (mysql_real_query(mysql_, "commit\n", 6)) {
        LOG(ERROR) << "mysql_real_query failed: " << mysql_error(mysql_) << " sql: commit";
        return false;
    }

    return true;
}

uint32_t MysqlConn::GetInsertId() { return (uint32_t) mysql_insert_id(mysql_); }

MysqlPool::MysqlPool(MysqlInfo info) : info_(std::move(info)) {}

MysqlPool::~MysqlPool() {
    exiting_ = true;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        cond_var_.notify_all();  // 通知所有在等待的
    }

    {
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto ptr: free_list_) {
            delete ptr;
        }
    }

    free_list_.clear();
    cur_conn_cnt_ = 0;
}

int MysqlPool::Init() {
    for (int i = 0; i < cur_conn_cnt_; i++) {
        auto *conn_ptr = new MysqlConn(info_);
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

MysqlConn *MysqlPool::GetMysqlConn(uint64_t timeout_ms) {
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
            auto *conn_ptr = new MysqlConn(info_);
            if (conn_ptr->Init()) {
                delete conn_ptr;
                return nullptr;
            } else {
                free_list_.push_back(conn_ptr);
                cur_conn_cnt_++;
            }
        }
    }
    MysqlConn *ptr = free_list_.front();
    free_list_.pop_front();

    return ptr;
}

void MysqlPool::RelMysqlConn(MysqlConn *conn) {
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

int MysqlManager::Init(const std::vector<MysqlInfo> &mysql_info) {
    infos_ = mysql_info;

    if (infos_.empty()) {
        LOG(ERROR) << "redis instance empty";
        return -1;
    }

    for (const MysqlInfo &info: infos_) {
        if (info.host.empty() || (info.port <= 0 || info.port > 65535) ||
            info.db_name.empty() || info.max_conn == 0) {
            LOG(ERROR) << "init redis poll param error, host: " << info.host
                       << "port: " << info.port << "db_name: " << info.db_name << "maxConn"
                       << info.max_conn;
            return -2;
        }
        auto *pool_ptr = new MysqlPool(info);
        if (pool_ptr->Init() != 0) {
            LOG(ERROR) << "init redis pool failed";
            return -3;
        }
        pool_map_.insert(make_pair(info.name, pool_ptr));
    }

    return 0;
}

MysqlConn *MysqlManager::GetMysqlConn(const std::string &pool_name) {
    auto ite = pool_map_.find(pool_name);
    if (ite == pool_map_.end()) {
        return nullptr;
    }

    return ite->second->GetMysqlConn();
}

void MysqlManager::RelMysqlConn(MysqlConn *conn) {
    if (!conn) {
        return;
    }

    auto ite = pool_map_.find(conn->GetPoolName());
    if (ite != pool_map_.end()) {
        return ite->second->RelMysqlConn(conn);
    }
}
