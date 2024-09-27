//
// Created by root on 9/26/24.
//

#include "Register.h"

#include <glog/logging.h>

#include "db/mysql/MysqlPool.h"

template<typename... Args>
std::string formatString2(const std::string &format, Args... args) {
    auto size = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;
    std::unique_ptr<char[]> buf(new char[size]);
    std::snprintf(buf.get(), size, format.c_str(), args...);
    return std::string(buf.get(), buf.get() + size - 1);
}

void Register::parse() {
    struct_json::from_json(info_, body_str_);
    assert(!info_.userName.empty());
    assert(!info_.firstPwd.empty());
}

std::string Register::RegUser() {
    int code = 0;
    uint32_t user_id = 0;

    MysqlConn *mysql_conn = MysqlManagerIns()->GetMysqlConn("master");
    AutoRelMysqlConn auto_rel_mysql(mysql_conn);
    // 先查看用户是否存在
    std::string str_sql = formatString2("select * from user_info where user_name='%s'", info_.userName.c_str());
    MysqlResultSet *result_set = mysql_conn->ExecuteQuery(str_sql.c_str());
    if (result_set && result_set->Next()) { // 检测是否存在用户记录
        // 存在在返回
        delete result_set;
        code = 2;
    } else {
        // 如果不存在则注册
        char create_time[32] = {0};
        //获取当前时间
        time_t now = time(nullptr);
        strftime(create_time, sizeof(create_time) - 1, "%Y-%m-%d %H:%M:%S", localtime(&now));
        str_sql = "insert into user_info "
                  "(`user_name`,`nick_name`,`password`,`phone`,`email`,`create_"
                  "time`) values(?,?,?,?,?,?)";
        LOG(ERROR) << "执行: " << str_sql;
        // MysqlPreStmt对象，否则有可能多个线程操作mysql对象，会crash
        auto *stmt = new MysqlPreStmt();
        if (stmt->Init(mysql_conn->GetMysql(), str_sql)) {
            uint32_t index = 0;
            std::string c_time = create_time;
            stmt->SetParam(index++, info_.userName);
            stmt->SetParam(index++, info_.nickName);
            stmt->SetParam(index++, info_.firstPwd);
            stmt->SetParam(index++, info_.phone);
            stmt->SetParam(index++, info_.email);
            stmt->SetParam(index++, c_time);
            bool bRet = stmt->ExecuteUpdate();
            if (bRet) {
                code = 0;
                user_id = mysql_conn->GetInsertId();
                LOG(ERROR) << "insert user_id: " << user_id;
            } else {
                LOG(ERROR) << "insert user_info failed. " << str_sql;
                code = 1;
            }
        }
        delete stmt;
    }

    std::string result;
    RegisterResult reg_result{};
    reg_result.code = code;
    struct_json::to_json(reg_result, result);
    return result;
}
