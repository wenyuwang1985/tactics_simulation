#include "AppConstants.h"

#include "log/Log.h"
#include "config/ClientConfig.h"

#include "test.h"

int main(int argc, char* argv[]) {
    logging::init(
        app::APP_LOG_NAME,
        app::APP_LOG_FILE,
        spdlog::level::debug
    );

    LOG_INFO("system start");

    // if (!config::ClientConfig::instance().load(app::CONFIG_FILE_PATH)) {
    //     LOG_ERROR("Failed to load config file!");
    //     spdlog::shutdown();
    //     return 0;
    // }

    test01();

    LOG_INFO("system end");
    spdlog::shutdown();
    return 0;
}
