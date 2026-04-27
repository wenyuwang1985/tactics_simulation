#include "test.h"

#include <string>

#include "log/Log.h"
#include "config/ClientConfig.h"

#include "TacticsReader.h"
//#include "MessageSender.h"
#include "ResultReceiver.h"
#include "TestEvent.h"

void test01() {
    const std::string tactics_file_path =
        config::ClientConfig::instance().tactics().file_path;
    if (tactics_file_path.empty()) {
        LOG_ERROR("Missing tactics_config.file_path in config");
        spdlog::shutdown();
        return;
    }

    tactics::InitialCondition condition;
    if (!tactics::loadInitialCondition(tactics_file_path, condition)) {
        LOG_ERROR("Failed to load initial condition: {}", tactics_file_path);
        spdlog::shutdown();
        return;
    }

    LOG_INFO("Loaded {} red entities, {} blue entities",
             condition.red.entity_list.size(),
             condition.blue.entity_list.size());

    // test_dds();
    test_redis();
    runLibeventTest();
}
