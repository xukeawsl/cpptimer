#ifndef _CPPTIMER_H_
#define _CPPTIMER_H_

#include <thread>
#include <atomic>
#include <chrono>
#include <mutex>
#include <functional>

namespace cpptimer
{

class Timer {
private:
    mutable std::mutex m;
    bool active_;
    long interval_;
    std::function<void()> cb_func;

public:
    Timer();
    ~Timer();
    template<class Function, class ...Args>
    void setTimeoutCallBack(Function&& func, Args&& ...args);
    void setInterval(long milliseconds);

    void start();
    void start(long milliseconds);
    void stop();

    long interval() const;
    bool isActive() const;
};

Timer::Timer() : active_(false), interval_(0) {
    cb_func = []{};
}

template<class Function, class ...Args>
inline void Timer::setTimeoutCallBack(Function&& func, Args&& ...args) {
    std::unique_lock<std::mutex> lk(m);
    cb_func = std::bind(std::forward<Function>(func), std::forward<Args>(args)...);
}

void Timer::setInterval(long milliseconds) {
    std::unique_lock<std::mutex> lk(m);
    interval_ = milliseconds;
}

void Timer::start() {
    start(interval_);
}

void Timer::start(long milliseconds) {
    std::unique_lock<std::mutex> lk(m);
    interval_ = milliseconds;
    if (active_) {
        return;
    }   
    active_ = true;
    std::thread t = std::thread([this](){
        while (true) {
            if (!this->isActive()) return;
            std::this_thread::sleep_for(std::chrono::milliseconds(this->interval_));
            this->cb_func();
        }
    });
    t.detach();
}

inline void Timer::stop() {
    std::unique_lock<std::mutex> lk(m);
    active_ = false;
}

inline long Timer::interval() const {
    std::unique_lock<std::mutex> lk(m);
    return interval_;
}

inline bool Timer::isActive() const {
    std::unique_lock<std::mutex> lk(m);
    return active_;    
}

Timer::~Timer() {
    stop();
}

}

#endif