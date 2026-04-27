#include "log/Log.h"

#include <spdlog/async.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <mutex>

namespace logging {

constexpr char*         DEFAULT_NAME          = "app";
constexpr char*         DEFAULT_LOG_FILE      = "app.log";
constexpr size_t        DEFAULT_MAX_FILE_SIZE = 10 * 1024 * 1024;  // 5MB
constexpr size_t        DEFAULT_MAX_FILES     = 3;
constexpr size_t        DEFAULT_QUEUE_SIZE     = 16384;
constexpr size_t        DEFAULT_THREAD_COUNT   = 1;
constexpr spdlog::level::level_enum DEFAULT_LOG_LEVEL = spdlog::level::info;

static std::shared_ptr<spdlog::async_logger> g_logger; // 改为具体类型，避免多态开销
static std::once_flag g_once;

bool init(const char* name,
          const char* logfile,
          spdlog::level::level_enum level) {

    std::call_once(g_once, [&]() {
        // 1. 初始化全局线程池 (8192 队列长度, 1 个后台线程)
        // 使用 spdlog::init_thread_pool 比手动创建更安全
        spdlog::init_thread_pool(DEFAULT_QUEUE_SIZE, DEFAULT_THREAD_COUNT);

        // 2. 创建 Sink
        auto sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            logfile, DEFAULT_MAX_FILE_SIZE, DEFAULT_MAX_FILES);

        // 3. 创建异步 Logger (自动使用全局线程池)
        // 注意这里使用 spdlog::create_async 而不是 new async_logger
        g_logger = std::make_shared<spdlog::async_logger>(
            name, 
            sink, 
            spdlog::thread_pool(), // 获取刚才 init 的全局池
            spdlog::async_overflow_policy::block
        );

        g_logger->set_level(level);
        g_logger->flush_on(level);
        
        // 打印文件名和行号的 pattern
        //g_logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%s:%# %!] %v");
        g_logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [tid %t] [%l] [%g:%# %!] %v");

        spdlog::register_logger(g_logger);
    });

    return g_logger != nullptr;
}

spdlog::logger* get() {
    return g_logger.get();
}

} // namespace logging