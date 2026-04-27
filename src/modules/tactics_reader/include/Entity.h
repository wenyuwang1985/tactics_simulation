#pragma once

#include <string>
#include <vector>

#include <json.hpp>

namespace tactics {

enum class EntityType {
    PLANE,
    SHIP,
    GROUND_EQUIPMENT,
    UNKNOWN
};

struct Ability {
    bool find   = false;
    bool fix    = false;
    bool track  = false;
    bool target = false;
    bool attack = false;
    bool assess = false;
};

struct Entity {
    std::string name;
    std::string id;
    EntityType  type = EntityType::UNKNOWN;
    Ability     ability;
};

struct SideConfig {
    std::vector<Entity> entity_list;
};

struct InitialCondition {
    SideConfig red;
    SideConfig blue;
};

void from_json(const nlohmann::json& j, Ability& a);
void from_json(const nlohmann::json& j, Entity& e);
void from_json(const nlohmann::json& j, SideConfig& s);
void from_json(const nlohmann::json& j, InitialCondition& ic);

}  // namespace tactics
