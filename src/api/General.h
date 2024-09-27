#pragma once

#include <string>

#include "db/mysql/MysqlPool.h"

class General {
public:
    static int DBGetUserFilesCountByUserName(MysqlConn *db_conn, const std::string &user_name, int64_t &count);
    static int DBGetSharePictureCountByUserName(MysqlConn *db_conn, const std::string &user_name, int64_t &count);
};
