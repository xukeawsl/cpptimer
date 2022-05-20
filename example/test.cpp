#include "cpptimer.h"

#include <iostream>

void func() {
    std::cout << "call_back" << std::endl;
}


int main(int argc, char* argv[]) {
    std::cout << sizeof(std::function<void()>) << std::endl;
    std::function<void()> cb_func = func;
    std::cout << sizeof(cb_func) << std::endl;

    cpptimer::Timer t;
    t.setTimeoutCallBack(cb_func);
    t.start(1000);

    std::this_thread::sleep_for(std::chrono::seconds(3));
    t.setInterval(500);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    t.stop();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "stop" << std::endl;
    return 0;
}