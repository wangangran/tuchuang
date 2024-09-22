#pragma once

#include <string>
#include <ylt/struct_yaml/yaml_reader.h>
#include <ylt/struct_yaml/yaml_writer.h>

struct MysqlInfo {
    std::string host;
    uint16_t port;
    std::string db_name;
    std::string user_name;
    std::string passwd;
    uint16_t max_conn;
};
YLT_REFL(MysqlInfo, host, port, db_name, user_name, passwd, max_conn);

struct RedisInfo {
    std::string name;
    std::string host;
    uint16_t port;
    std::string passwd;
    uint16_t db;
    uint16_t max_conn;
};
YLT_REFL(RedisInfo, name, host, port, db, max_conn);

struct TcConfInfo {
    std::string log_dir;
    std::string http_listen_ip;
    uint16_t http_port;
    int thread_num;
    std::string log_level;
    std::string web_domain;
    std::string web_port;
    std::string db_instances;
    MysqlInfo sql_master;
    MysqlInfo sql_slave;
    std::vector<RedisInfo> redis_info;
};
YLT_REFL(TcConfInfo, log_dir, http_listen_ip, http_port, thread_num, log_level, web_domain, web_port, db_instances, sql_master, sql_slave, redis_info);

class TcConf {
public:
    explicit TcConf(const std::string &conf_path);
    int parse();
    
    TcConfInfo info_;
    std::string conf_path_;
};