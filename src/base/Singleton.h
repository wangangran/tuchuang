#pragma once

#include <atomic>
#include <thread>
#include <mutex>

template<typename T>
class Singleton {
public:
    static T *instance() {
        static std::mutex mtx;
        static std::atomic<T*> ins(nullptr);

        T *tmp = ins.load();
        if (nullptr == tmp) {
            std::lock_guard<std::mutex> lock(mtx);
            if (nullptr == tmp) {
                tmp = new T;
                // 加入内存屏障
                ins.store(tmp);
            }
        }
        return ins.load();
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
private:
    Singleton() {}
};
