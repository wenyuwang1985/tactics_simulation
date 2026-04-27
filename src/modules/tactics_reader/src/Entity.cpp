#include "Entity.h"

#include <unordered_map>

namespace tactics {

static EntityType parseEntityType(const std::string& s) {
    static const std::unordered_map<std::string, EntityType> map = {
        {"plane",            EntityType::PLANE},
        {"ship",             EntityType::SHIP},
        {"ground_equipment", EntityType::GROUND_EQUIPMENT},
    };
    auto it = map.find(s);
    return (it != map.end()) ? it->second : EntityType::UNKNOWN;
}

void from_json(const nlohmann::json& j, Ability& a) {
    a.find   = j.value("find",   false);
    a.fix    = j.value("fix",    false);
    a.track  = j.value("track",  false);
    a.target = j.value("target", false);
    a.attack = j.value("attack", false);
    a.assess = j.value("assess", false);
}

void from_json(const nlohmann::json& j, Entity& e) {
    e.name = j.value("name", "");
    e.id   = j.value("id",   "");
    e.type = parseEntityType(j.value("type", ""));

    if (j.contains("ability")) {
        e.ability = j.at("ability").get<Ability>();
    }
    // else: ability keeps default (all false)
}

void from_json(const nlohmann::json& j, SideConfig& s) {
    if (j.contains("entity_list")) {
        s.entity_list = j.at("entity_list").get<std::vector<Entity>>();
    }
}

void from_json(const nlohmann::json& j, InitialCondition& ic) {
    if (j.contains("red")) {
        ic.red = j.at("red").get<SideConfig>();
    }
    if (j.contains("blue")) {
        ic.blue = j.at("blue").get<SideConfig>();
    }
}

}  // namespace tactics
