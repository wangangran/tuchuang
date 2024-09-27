#include "General.h"

#include <glog/logging.h>

#include "base/Common.h"

//获取用户文件个数
int General::DBGetUserFilesCountByUserName(MysqlConn *db_conn, const std::string &user_name, int64_t &count) {
    count = 0;
    std::string str_sql = Common::formatString("select count(*) from user_file_list where user='%s'",
                                               user_name.c_str());
    LOG(ERROR) << "执行: " << str_sql;
    MysqlResultSet *result_set = db_conn->ExecuteQuery(str_sql.c_str());
    if (result_set == nullptr) {
        LOG(ERROR) << str_sql << " 操作失败";
        return -1;
    }

    if (result_set->Next()) {
        count = result_set->GetInt("count(*)");
        LOG(INFO) << "count: " << count;
    } else {
        // 没有记录则初始化记录数量为0
        LOG(INFO) << "没有记录: count: " << count;
    }
    delete result_set;
    return 0;
}

//获取用户共享图片格式
int General::DBGetSharePictureCountByUserName(MysqlConn *db_conn, const std::string &user_name, int64_t &count) {
    count = 0;
    std::string str_sql = Common::formatString("select count(*) from share_picture_list where user='%s'",
                                               user_name.c_str());
    LOG(ERROR) << "执行: " << str_sql;
    MysqlResultSet *result_set = db_conn->ExecuteQuery(str_sql.c_str());
    if (result_set == nullptr) {
        LOG(ERROR) << str_sql << " 操作失败";
        return -1;
    }

    if (result_set->Next()) {
        count = result_set->GetInt("count(*)");
        LOG(INFO) << "count: " << count;
    } else {
        // 没有记录则初始化记录数量为0
        LOG(INFO) << "没有记录: count: " << count;
    }
    delete result_set;
    return 0;
}
