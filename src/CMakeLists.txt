
add_executable(tuchuang main.cpp db/redis/RedisPool.cpp conf/TcConf.cpp
        HttpConn.cpp
        api/Register.cpp
        db/mysql/MysqlPool.cpp)
target_link_libraries(tuchuang glog hiredis mysqlclient)