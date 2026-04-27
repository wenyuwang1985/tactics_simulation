#include "test.h"

#include "AppConstants.h"

#include "log/Log.h"
#include "config/ClientConfig.h"

#include "TacticsReader.h"
//#include "MessageSender.h"
#include "ResultReceiver.h"
#include "TestEvent.h"

void test01() {
    tactics::InitialCondition condition;
    if (!tactics::loadInitialCondition(app::TACTICS_CONFIG_FILE_PATH, condition)) {
        LOG_ERROR("Failed to load initial condition");
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