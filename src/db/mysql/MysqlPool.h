#pragma once

#include <mysql/mysql.h>
#include <condition_variable>
#include <list>
#include <map>
#include <mutex>
#include <vector>

#include "base/Singleton.h"
#include "conf/TcConf.h"

// 返回结果 select的时候用
class MysqlResultSet {
public:
    explicit MysqlResultSet(MYSQL_RES *res);

    virtual ~MysqlResultSet();

    bool Next();

    int GetInt(const char *key);

    char *GetString(const char *key);

private:
    int GetIndex(const char *key);

    // 该结构代表返回行的查询结果（SELECT, SHOW, DESCRIBE, EXPLAIN）
    MYSQL_RES *res_ = nullptr;
    // 这是1行数据的“类型安全”表示。它目前是按照计数字节字符串的数组实施的。
    MYSQL_ROW row_{};
    std::map<std::string, int> key_map_;
};

// 插入数据用
class MysqlPreStmt {
public:
    MysqlPreStmt();

    virtual ~MysqlPreStmt();

    bool Init(MYSQL *mysql, std::string &sql);

    void SetParam(uint32_t index, int &value);

    void SetParam(uint32_t index, uint32_t &value);

    void SetParam(uint32_t index, std::string &value);

    void SetParam(uint32_t index, const std::string &value);

    bool ExecuteUpdate();

    uint32_t GetInsertId();

private:
    MYSQL_STMT *stmt_;
    MYSQL_BIND *param_bind_;
    uint32_t param_cnt_;
};

class MysqlConn {
public:
    explicit MysqlConn(MysqlInfo info);

    ~MysqlConn();

    int Init();

    void Release();

    // 创建表
    bool ExecuteCreate(const char *sql_query);

    // 删除表
    bool ExecuteDrop(const char *sql_query);

    // 查询
    MysqlResultSet *ExecuteQuery(const char *sql_query);

    bool ExecutePassQuery(const char *sql_query);

    // care_affected_rows 是否在意影响的行数，false:不在意；true:在意
    bool ExecuteUpdate(const char *sql_query, bool care_affected_rows = true);

    // 开启事务
    bool StartTransaction();

    // 提交事务
    bool Commit();

    // 回滚事务
    bool Rollback();

    // 获取连接池名
    [[nodiscard]] std::string GetPoolName() const { return info_.name; }

    [[nodiscard]] MYSQL *GetMysql() const { return mysql_; }

    uint32_t GetInsertId();

    [[nodiscard]] uint64_t GetRowNum() const { return row_num_; }

private:
    MYSQL *mysql_ = nullptr;
    MysqlInfo info_;
    uint64_t row_num_ = 0;
};

class MysqlPool {
public:
    explicit MysqlPool(MysqlInfo info);

    ~MysqlPool();

    int Init();

    MysqlConn *GetMysqlConn(uint64_t timeout_ms = 0);

    void RelMysqlConn(MysqlConn *conn);

private:
    std::atomic<bool> exiting_ = false;
    std::mutex mutex_;
    std::condition_variable cond_var_;
    MysqlInfo info_;
    int cur_conn_cnt_ = 2;
    std::list<MysqlConn *> free_list_;
};

class MysqlManager {
public:
    int Init(const std::vector<MysqlInfo> &mysql_info);

    MysqlConn *GetMysqlConn(const std::string &pool_name);

    void RelMysqlConn(MysqlConn *conn);

private:
    std::string cacheInstances_;
    std::vector<MysqlInfo> infos_;
    std::map<std::string, MysqlPool *> pool_map_;
};

#define MysqlManagerIns Singleton<MysqlManager>::instance

// 目的是在函数退出后自动将连接归还连接池
class AutoRelMysqlConn {
public:
    explicit AutoRelMysqlConn(MysqlConn *conn) : conn_(conn) {}

    ~AutoRelMysqlConn() {
        MysqlManagerIns()->RelMysqlConn(conn_);
    }

private:
    MysqlConn *conn_ = nullptr;
};
