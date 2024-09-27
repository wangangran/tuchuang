
add_executable(tuchuang main.cpp db/redis/RedisPool.cpp conf/TcConf.cpp
        HttpConn.cpp
        api/Register.cpp
        db/mysql/MysqlPool.cpp
        api/Login.cpp
        api/General.cpp
        api/General.cpp
        api/General.cpp)
target_link_libraries(tuchuang glog hiredis mysqlclient)