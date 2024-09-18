//
// Created by root on 9/18/24.
//

#ifndef TUCHUANG_SRC_CONF_TCCONF_H
#define TUCHUANG_SRC_CONF_TCCONF_H

#include <string>
#include <ylt/struct_yaml/yaml_reader.h>
#include <ylt/struct_yaml/yaml_writer.h>

struct MysqlInfo {
    std::string host;
    uint16_t port;
    std::string dbName;
    std::string userName;
    std::string passwd;
    uint16_t maxConn;
};
YLT_REFL(MysqlInfo, host, port, dbName, userName, passwd, maxConn);

struct RedisInfo {
    std::string host;
    uint16_t port;
    uint16_t db;
    uint16_t maxConn;
};
YLT_REFL(RedisInfo, host, port, db, maxConn);

struct TcConfInfo {
    std::string httpListenIp;
    uint16_t httpPort;
    int threadNum;
    std::string logLevel;
    std::string webDomain;
    std::string webPort;
    std::string dbInstances;
    MysqlInfo sqlMaster;
    MysqlInfo sqlSlave;
    std::string cacheInstances;
    RedisInfo redisToken;
    RedisInfo redisRank;   
};
YLT_REFL(TcConfInfo, httpListenIp, httpPort, threadNum, logLevel, webDomain, webPort, dbInstances, sqlMaster, sqlSlave, cacheInstances, redisToken, redisRank);

class TcConf {
public:
    explicit TcConf(const std::string &confPath);
    int parse();
    
    std::string confPath_;
};


#endif // TUCHUANG_SRC_CONF_TCCONF_H
