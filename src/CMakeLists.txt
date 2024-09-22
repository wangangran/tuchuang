
add_executable(tuchuang main.cpp conf/TcConf.cpp db/redis/RedisPool.cpp)

target_link_libraries(tuchuang glog hiredis)