#include "config/ClientConfig.h"
#include <ryml.hpp>
#include <ryml_std.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

namespace config {

static AuthMode stringToAuthMode(const std::string& mode) {
    if (mode == "cookie") return AuthMode::Cookie;
    if (mode == "bearer") return AuthMode::Bearer;
    return AuthMode::None;
}

// 从 ryml csubstr 安全提取 std::string
static std::string toStdString(c4::csubstr s) {
    return std::string(s.data(), s.size());
}

// 解析单个模块节点的 endpoints
static void parseModule(const c4::yml::ConstNodeRef& node,
                        ModuleConfig& out, AuthMode systemMode) {
    out.auth_mode = systemMode;

    if (node.has_child("endpoints")) {
        for (c4::yml::ConstNodeRef::const_iterator it = node["endpoints"].begin();
             it != node["endpoints"].end(); ++it) {
            c4::yml::ConstNodeRef ep = *it;
            std::string key = toStdString(ep.key());
            std::string val = toStdString(ep.val());
            out.endpoints[key] = val;
        }
    }
}

ClientConfig& ClientConfig::instance() {
    static ClientConfig inst;
    return inst;
}

bool ClientConfig::load(const std::string& filePath) {
    servers_.clear();
    biz_ = BizConfig();
    conflict_ = ConflictConfig();
    timeoutMs_ = 5000;
    connectTimeoutMs_ = 3000;
    retryIntervalMs_ = 1000;

    std::ifstream ifs(filePath, std::ios::binary);
    if (!ifs.is_open()) {
        return false;
    }

    std::stringstream ss;
    ss << ifs.rdbuf();
    std::string content = ss.str();

    try {
        ryml::Tree tree = ryml::parse_in_arena(ryml::to_csubstr(content));
        ryml::ConstNodeRef root = tree.rootref();

        // 1. 网络设置
        if (root.has_child("network")) {
            ryml::ConstNodeRef net = root["network"];
            if (net.has_child("timeout_ms")) {
                net["timeout_ms"] >> timeoutMs_;
            }
            if (net.has_child("connect_timeout_ms")) {
                net["connect_timeout_ms"] >> connectTimeoutMs_;
            }
            if (net.has_child("retry_interval_ms")) {
                net["retry_interval_ms"] >> retryIntervalMs_;
            }

            // 服务器地址列表
            if (net.has_child("servers")) {
                ryml::ConstNodeRef serversNode = net["servers"];
                for (size_t i = 0; i < serversNode.num_children(); ++i) {
                    ryml::ConstNodeRef entry = serversNode[i];
                    ServerEntry se;
                    if (entry.has_child("name")) {
                        se.name = toStdString(entry["name"].val());
                    }
                    if (entry.has_child("url")) {
                        se.url = toStdString(entry["url"].val());
                    }
                    if (!se.url.empty()) {
                        servers_.push_back(se);
                    }
                }
            }
        }

        // 2. 系统配置
        if (root.has_child("systems")) {
            ryml::ConstNodeRef systems = root["systems"];

            // biz 系统
            if (systems.has_child("biz")) {
                ryml::ConstNodeRef bizNode = systems["biz"];

                std::string modeStr;
                if (bizNode.has_child("auth_mode")) {
                    bizNode["auth_mode"] >> modeStr;
                }
                AuthMode bizMode = stringToAuthMode(modeStr);
                biz_.auth_mode = bizMode;

                if (bizNode.has_child("modules")) {
                    ryml::ConstNodeRef mods = bizNode["modules"];
                    if (mods.has_child("auth"))
                        parseModule(mods["auth"], biz_.auth, bizMode);
                    if (mods.has_child("probe"))
                        parseModule(mods["probe"], biz_.probe, bizMode);
                    if (mods.has_child("menu"))
                        parseModule(mods["menu"], biz_.menu, bizMode);
                    if (mods.has_child("fabric"))
                        parseModule(mods["fabric"], biz_.fabric, bizMode);
                }
            }

            // conflict 系统（独立 base_url）
            if (systems.has_child("conflict")) {
                ryml::ConstNodeRef confNode = systems["conflict"];

                if (confNode.has_child("base_url")) {
                    conflict_.base_url = toStdString(confNode["base_url"].val());
                }

                std::string modeStr;
                if (confNode.has_child("auth_mode")) {
                    confNode["auth_mode"] >> modeStr;
                }
                AuthMode confMode = stringToAuthMode(modeStr);
                conflict_.auth_mode = confMode;

                if (confNode.has_child("modules")) {
                    ryml::ConstNodeRef mods = confNode["modules"];
                    if (mods.has_child("analysis"))
                        parseModule(mods["analysis"], conflict_.analysis, confMode);
                }
            }
        }

        return true;
    } catch (const std::exception& e) {
        std::cerr << "Config Parse Error: " << e.what() << std::endl;
        return false;
    }
}

} // namespace config
