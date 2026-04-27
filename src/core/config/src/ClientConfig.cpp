#include "config/ClientConfig.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include <ryml.hpp>
#include <ryml_std.hpp>

namespace config {

namespace {

constexpr char REDIS_NODE[] = "redis";
constexpr char TACTICS_NODE[] = "tactics_config";
constexpr char HOST_NODE[] = "host";
constexpr char PORT_NODE[] = "port";
constexpr char PASSWORD_NODE[] = "password";
constexpr char DATABASE_NODE[] = "database";
constexpr char TIMEOUT_NODE[] = "timeout";
constexpr char FILE_PATH_NODE[] = "file_path";

std::string toStdString(c4::csubstr value) {
    return std::string(value.data(), value.size());
}

bool openConfigFile(const std::string& file_path, std::ifstream& input_stream) {
    input_stream.open(file_path, std::ios::binary);
    return input_stream.is_open();
}

std::string readConfigContent(std::ifstream& input_stream) {
    std::stringstream content_stream;
    content_stream << input_stream.rdbuf();
    return content_stream.str();
}

void parseRedisConfig(
    const c4::yml::ConstNodeRef& redis_node,
    RedisConfig& redis_config
) {
    if (redis_node.has_child(HOST_NODE)) {
        redis_config.host = toStdString(redis_node[HOST_NODE].val());
    }
    if (redis_node.has_child(PORT_NODE)) {
        redis_node[PORT_NODE] >> redis_config.port;
    }
    if (redis_node.has_child(PASSWORD_NODE)) {
        redis_config.password = toStdString(redis_node[PASSWORD_NODE].val());
    }
    if (redis_node.has_child(DATABASE_NODE)) {
        redis_node[DATABASE_NODE] >> redis_config.database;
    }
    if (redis_node.has_child(TIMEOUT_NODE)) {
        redis_node[TIMEOUT_NODE] >> redis_config.timeout_ms;
    }
}

void parseTacticsConfig(
    const c4::yml::ConstNodeRef& tactics_node,
    TacticsConfig& tactics_config
) {
    if (!tactics_node.has_child(FILE_PATH_NODE)) {
        return;
    }
    tactics_config.file_path = toStdString(tactics_node[FILE_PATH_NODE].val());
}

} // namespace

ClientConfig& ClientConfig::instance() {
    static ClientConfig instance;
    return instance;
}

bool ClientConfig::load(const std::string& file_path) {
    reset();

    std::ifstream input_stream;
    if (!openConfigFile(file_path, input_stream)) {
        return false;
    }

    const std::string content = readConfigContent(input_stream);

    try {
        ryml::Tree tree = ryml::parse_in_arena(ryml::to_csubstr(content));
        const ryml::ConstNodeRef root = tree.rootref();

        if (root.has_child(REDIS_NODE)) {
            parseRedisConfig(root[REDIS_NODE], redis_config_);
        }
        if (root.has_child(TACTICS_NODE)) {
            parseTacticsConfig(root[TACTICS_NODE], tactics_config_);
        }
        return true;
    } catch (const std::exception& exception) {
        std::cerr << "Config parse error: " << exception.what() << std::endl;
        return false;
    }
}

void ClientConfig::reset() {
    redis_config_ = RedisConfig();
    tactics_config_ = TacticsConfig();
}

} // namespace config
