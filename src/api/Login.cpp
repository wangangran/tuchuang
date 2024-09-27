#include "Login.h"

#include <glog/logging.h>

#include "api/General.h"
#include "base/Common.h"
#include "db/mysql/MysqlPool.h"
#include "db/redis/RedisPool.h"

void Login::DoLogin() {
    // 判断数据是否为空
    if (body_str_.empty()) {
        return;
    }

    // 验证账号和密码是否匹配
    if (VerifyUserPassword() < 0) {
        LOG(ERROR) << "verifyUserPassword failed";
        return;
    }

    // 生成token
    if (SetToken() < 0) {
        LOG(ERROR) << "setToken failed";
        return;
    }

    // 加载 我的文件数量  我的分享图片数量
    if (LoadMyFilesCountAndSharePictureCount() < 0) {
        LOG(ERROR) << "LoadMyFilesCountAndSharePictureCount failed";
        return;
    }

    resp_.code = 0;
}

std::string Login::GetResult() {
    std::string result;
    struct_json::to_json(resp_, result);
    return result;
}

void Login::Parse() {
    struct_json::from_json(resp_, body_str_);
    assert(!req_.user.empty());
    assert(!req_.pwd.empty());
}

int Login::VerifyUserPassword() const {
    int ret = 0;
    MysqlConn *mysql_conn = MysqlManagerIns()->GetMysqlConn("master");
    AutoRelMysqlConn auto_rel_mysql(mysql_conn);

    // 先查看用户是否存在
    std::string sql_str = Common::formatString("select password from user_info where user_name='%s'",
                                               req_.user.c_str());
    MysqlResultSet *result_set = mysql_conn->ExecuteQuery(sql_str.c_str());
    if (result_set && result_set->Next()) {
        std::string password = result_set->GetString("password");
        LOG(INFO) << "mysql-pwd: " << password << " user-pwd: " << req_.pwd;
        if (result_set->GetString("password") == req_.pwd) {
            ret = 0;
        } else {
            ret = -1;
        }
    } else {
        ret = -1;
    }

    delete result_set;

    return ret;
}

int Login::SetToken() {
    RedisConn *redis_conn = RedisManagerIns()->GetRedisConn("token");
    if (redis_conn == nullptr) {
        return -1;
    }

    AutoRelRedisConn auto_rel_redis(redis_conn);

    resp_.token = Common::RandomString(32); // 随机32个字母

    redis_conn->SetEx(req_.user, resp_.token, 86400); // redis做超时, 24小时
    return 0;
}

int Login::LoadMyFilesCountAndSharePictureCount() const {
    int64_t file_count = 0;

    MysqlConn *mysql_conn = MysqlManagerIns()->GetMysqlConn("master");
    if (mysql_conn == nullptr) {
        return -1;
    }

    RedisConn *redis_conn = RedisManagerIns()->GetRedisConn("token");
    if (redis_conn == nullptr) {
        return -1;
    }

    AutoRelMysqlConn auto_rel_mysql(mysql_conn);
    AutoRelRedisConn auto_rel_redis(redis_conn);

    // 从mysql加载
    if (General::DBGetUserFilesCountByUserName(mysql_conn, req_.user, file_count) < 0) {
        LOG(ERROR) << "DBGetUserFilesCountByUserName failed";
        return -1;
    }

    if (redis_conn->Set("file_user_count" + req_.user, std::to_string(file_count)).empty()) {
        LOG(ERROR) << "set redis file user count failed, file count: " << file_count;
        return -1;
    }

    LOG(INFO) << "file_user_count: " << file_count;

    // 从mysql加载 我的分享图片数量
    if (General::DBGetSharePictureCountByUserName(mysql_conn, req_.user, file_count) < 0) {
        LOG(ERROR) << "DBGetSharePictureCountByUserName failed";
        return -1;
    }

    if (redis_conn->Set("shared_pic_count" + req_.user, std::to_string(file_count)).empty()) {
        LOG(ERROR) << "set redis shared pic count failed, file count: " << file_count;
        return -1;
    }

    LOG(INFO) << "shared_pic_count: " << file_count;

    return 0;
}
