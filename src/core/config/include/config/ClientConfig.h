#pragma once

#include <string>

namespace config {

struct RedisConfig {
    std::string host = "127.0.0.1";
    int port = 6379;
    std::string password;
    int database = 0;
    int timeout_ms = 5000;
};

struct TacticsConfig {
    std::string file_path;
};

class ClientConfig {
public:
    static ClientConfig& instance();

    ClientConfig(const ClientConfig&) = delete;
    ClientConfig& operator=(const ClientConfig&) = delete;

    bool load(const std::string& file_path);

    const RedisConfig& redis() const { return redis_config_; }
    const TacticsConfig& tactics() const { return tactics_config_; }

private:
    ClientConfig() = default;

    void reset();

    RedisConfig redis_config_;
    TacticsConfig tactics_config_;
};

} // namespace config
