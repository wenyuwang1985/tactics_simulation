#include "AppConstants.h"

#include "log/Log.h"
#include "config/ClientConfig.h"

#include "TacticsReader.h"
//#include "MessageSender.h"
#include "ResultReceiver.h"
#include "TestEvent.h"

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

    tactics::InitialCondition condition;
    if (!tactics::loadInitialCondition(app::TACTICS_CONFIG_FILE_PATH, condition)) {
        LOG_ERROR("Failed to load initial condition");
        spdlog::shutdown();
        return 1;
    }

    LOG_INFO("Loaded {} red entities, {} blue entities",
             condition.red.entity_list.size(),
             condition.blue.entity_list.size());

    // test_dds();
    test_redis();
    runLibeventTest();

    LOG_INFO("client end");
    spdlog::shutdown();
    return 0;
}
