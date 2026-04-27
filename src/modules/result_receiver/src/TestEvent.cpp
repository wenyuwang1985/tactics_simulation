#include "TestEvent.h"
#include <iostream>

// 引入 libevent 的核心头文件
#include <event2/event.h>

// 这是一个回调函数，当定时器超时时，libevent 会调用它
static void timer_callback(evutil_socket_t fd, short event, void *arg) {
    std::cout << "[TestEvent] 🕒 定时器被触发了！" << std::endl;
    std::cout << "[TestEvent] ✅ libevent 编译并运行完全正常！" << std::endl;
    // 因为这只是一个一次性事件（没有设置 EV_PERSIST），
    // 触发完之后，事件队列变空，event_base_dispatch 会自动退出。
}

void runLibeventTest() {
    std::cout << "[TestEvent] 🚀 正在初始化 libevent 测试..." << std::endl;

    // 1. 创建事件处理的全局基础结构 (event_base)
    struct event_base *base = event_base_new();
    if (!base) {
        std::cerr << "[TestEvent] ❌ 失败：无法创建 event_base!" << std::endl;
        return;
    }

    std::cout << "[TestEvent] libevent 方法使用: " << event_base_get_method(base) << std::endl;

    // 2. 创建一个定时器事件
    // evtimer_new 是 event_new(base, -1, 0, cb, arg) 的快捷宏
    struct event *timer_ev = evtimer_new(base, timer_callback, nullptr);
    if (!timer_ev) {
        std::cerr << "[TestEvent] ❌ 失败：无法创建定时器事件!" << std::endl;
        event_base_free(base);
        return;
    }

    // 3. 设置超时时间为 1 秒，并添加到事件循环中
    struct timeval tv;
    tv.tv_sec = 1;      // 1 秒
    tv.tv_usec = 0;     // 0 微秒
    evtimer_add(timer_ev, &tv);

    std::cout << "[TestEvent] ⏳ 进入事件循环，等待 1 秒钟..." << std::endl;

    // 4. 开始事件循环 (将阻塞在这里，直到事件触发或出错)
    event_base_dispatch(base);

    std::cout << "[TestEvent] 🛑 事件循环安全退出，测试结束。" << std::endl;

    // 5. 释放资源
    event_free(timer_ev);
    event_base_free(base);
}