#include "TacticsReader.h"

#include "log/Log.h"

#include <fstream>

namespace tactics {

bool loadInitialCondition(const std::string& filepath, InitialCondition& out) {
    std::ifstream ifs(filepath);
    if (!ifs.is_open()) {
        LOG_ERROR("Cannot open initial condition file: {}", filepath);
        return false;
    }

    nlohmann::json j;
    try {
        j = nlohmann::json::parse(ifs);
    } catch (const nlohmann::json::parse_error& e) {
        LOG_ERROR("JSON parse error in {}: {}", filepath, e.what());
        return false;
    }

    try {
        out = j.get<InitialCondition>();
    } catch (const nlohmann::json::exception& e) {
        LOG_ERROR("JSON structure error in {}: {}", filepath, e.what());
        return false;
    }

    LOG_INFO("Loaded initial condition from {}: red={} entities, blue={} entities",
             filepath,
             out.red.entity_list.size(),
             out.blue.entity_list.size());
    return true;
}

}  // namespace tactics
