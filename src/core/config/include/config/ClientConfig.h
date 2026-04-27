#pragma once

#include <string>
#include <vector>
#include <map>

namespace config {

enum class AuthMode {
    None,
    Cookie,
    Bearer
};

// 服务器地址条目（跨机房切换）
struct ServerEntry {
    std::string name;   // 显示名称，如 "机房A"
    std::string url;    // 基础地址，如 "http://192.168.110.100:8080"
};

// 单个模块的配置（如 auth, menu 等）
struct ModuleConfig {
    AuthMode auth_mode = AuthMode::None;
    std::map<std::string, std::string> endpoints;

    // 获取具体接口后缀
    std::string getEp(const std::string& key) const {
        std::map<std::string, std::string>::const_iterator it = endpoints.find(key);
        return (it != endpoints.end()) ? it->second : "";
    }
};

// 业务系统的模块集合
struct BizConfig {
    AuthMode auth_mode = AuthMode::None;  // 系统级认证模式
    ModuleConfig auth;
    ModuleConfig probe;
    ModuleConfig menu;
    ModuleConfig fabric;
};

// 冲突分析系统的模块集合
struct ConflictConfig {
    std::string base_url;                 // 独立系统有自己的 base_url
    AuthMode auth_mode = AuthMode::None;
    ModuleConfig analysis;
};

class ClientConfig {
public:
    static ClientConfig& instance();

    // 禁止拷贝
    ClientConfig(const ClientConfig&) = delete;
    ClientConfig& operator=(const ClientConfig&) = delete;

    // 加载配置文件
    bool load(const std::string& filePath);

    // 网络设置
    int timeoutMs() const { return timeoutMs_; }
    int connectTimeoutMs() const { return connectTimeoutMs_; }
    int retryIntervalMs() const { return retryIntervalMs_; }

    // 服务器地址列表（跨机房切换）
    const std::vector<ServerEntry>& servers() const { return servers_; }

    // 业务系统配置
    const BizConfig& biz() const { return biz_; }
    const ConflictConfig& conflict() const { return conflict_; }

private:
    ClientConfig() = default;

    int timeoutMs_ = 5000;
    int connectTimeoutMs_ = 3000;
    int retryIntervalMs_ = 1000;
    std::vector<ServerEntry> servers_;
    BizConfig biz_;
    ConflictConfig conflict_;
};

} // namespace config
