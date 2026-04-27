#include "ResultReceiver.h"

#include <hiredis.h>
#include <sys/time.h>

#include <cstdlib>
#include <iostream>
#include <string>

#include "config/ClientConfig.h"

namespace {

constexpr char TEST_KEY[] = "test_key";
constexpr char TEST_VALUE[] = "hello_from_dds";
constexpr char AUTH_COMMAND[] = "AUTH %s";
constexpr char SELECT_COMMAND[] = "SELECT %d";
constexpr char SET_COMMAND[] = "SET %s %s";
constexpr char GET_COMMAND[] = "GET %s";
constexpr char DELETE_COMMAND[] = "DEL %s";

timeval buildTimeout(int timeout_ms) {
    constexpr int MILLISECONDS_PER_SECOND = 1000;
    constexpr int MICROSECONDS_PER_MILLISECOND = 1000;
    timeval timeout_value{};
    timeout_value.tv_sec = timeout_ms / MILLISECONDS_PER_SECOND;
    timeout_value.tv_usec =
        (timeout_ms % MILLISECONDS_PER_SECOND) * MICROSECONDS_PER_MILLISECOND;
    return timeout_value;
}

redisContext* connectRedis(const config::RedisConfig& redis_config) {
    const timeval timeout_value = buildTimeout(redis_config.timeout_ms);
    return redisConnectWithTimeout(
        redis_config.host.c_str(),
        redis_config.port,
        timeout_value
    );
}

bool ensureContext(redisContext* redis_context) {
    if (redis_context == nullptr) {
        std::cerr << "Redis connect error: can't allocate redis context" << std::endl;
        return false;
    }
    if (redis_context->err == 0) {
        return true;
    }
    std::cerr << "Redis connect error(" << redis_context->err << "): "
              << redis_context->errstr << std::endl;
    redisFree(redis_context);
    return false;
}

bool authorize(redisContext* redis_context, const config::RedisConfig& redis_config) {
    if (redis_config.password.empty()) {
        return true;
    }
    redisReply* reply = static_cast<redisReply*>(
        redisCommand(redis_context, AUTH_COMMAND, redis_config.password.c_str())
    );
    if (reply == nullptr) {
        return false;
    }
    const bool is_success = reply->type != REDIS_REPLY_ERROR;
    freeReplyObject(reply);
    return is_success;
}

bool selectDatabase(
    redisContext* redis_context,
    const config::RedisConfig& redis_config
) {
    if (redis_config.database == 0) {
        return true;
    }
    redisReply* reply = static_cast<redisReply*>(
        redisCommand(redis_context, SELECT_COMMAND, redis_config.database)
    );
    if (reply == nullptr) {
        return false;
    }
    const bool is_success = reply->type != REDIS_REPLY_ERROR;
    freeReplyObject(reply);
    return is_success;
}

void printStringReply(const char* label, redisReply* reply) {
    if (reply == nullptr || reply->str == nullptr) {
        return;
    }
    std::cout << label << ": " << reply->str << std::endl;
}

void printIntegerReply(const char* label, redisReply* reply) {
    if (reply == nullptr) {
        return;
    }
    std::cout << label << ": " << reply->integer << std::endl;
}

void runRedisSample(redisContext* redis_context) {
    redisReply* set_reply = static_cast<redisReply*>(
        redisCommand(redis_context, SET_COMMAND, TEST_KEY, TEST_VALUE)
    );
    printStringReply("SET test_key", set_reply);
    freeReplyObject(set_reply);

    redisReply* get_reply = static_cast<redisReply*>(
        redisCommand(redis_context, GET_COMMAND, TEST_KEY)
    );
    printStringReply("GET test_key", get_reply);
    freeReplyObject(get_reply);

    redisReply* delete_reply = static_cast<redisReply*>(
        redisCommand(redis_context, DELETE_COMMAND, TEST_KEY)
    );
    printIntegerReply("DEL test_key", delete_reply);
    freeReplyObject(delete_reply);
}

} // namespace

void test_redis() {
    const config::RedisConfig& redis_config =
        config::ClientConfig::instance().redis();
    redisContext* redis_context = connectRedis(redis_config);
    if (!ensureContext(redis_context)) {
        std::cout << "Failed to connect to Redis" << std::endl;
        return;
    }
    if (!authorize(redis_context, redis_config)) {
        std::cout << "Failed to authorize with Redis" << std::endl;
        redisFree(redis_context);
        return;
    }
    if (!selectDatabase(redis_context, redis_config)) {
        std::cout << "Failed to select Redis database" << std::endl;
        redisFree(redis_context);
        return;
    }

    std::cout << "Connected to Redis successfully" << std::endl;
    runRedisSample(redis_context);
    redisFree(redis_context);
    std::cout << "Redis test done" << std::endl;
}
