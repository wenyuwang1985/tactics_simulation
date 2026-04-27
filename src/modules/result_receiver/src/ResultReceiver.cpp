#include "ResultReceiver.h"

#include <hiredis.h>
#include <iostream>

void test_redis() {
    redisContext *c = redisConnect("127.0.0.1", 6379);
    if (c == nullptr || c->err) {
        if (c) {
            std::cerr << "Redis connect error(" << c->err << "): " << c->errstr << std::endl;
            redisFree(c);
        } else {
            std::cerr << "Redis connect error: can't allocate redis context" << std::endl;
        }
        return;
    }
    
    std::cout << "Connected to Redis successfully" << std::endl;

    // SET
    redisReply *reply = static_cast<redisReply *>(redisCommand(c, "SET %s %s", "test_key", "hello_from_dds"));
    if (reply) {
        std::cout << "SET test_key: " << reply->str << std::endl;
        freeReplyObject(reply);
    }

    // GET
    reply = static_cast<redisReply *>(redisCommand(c, "GET %s", "test_key"));
    if (reply) {
        std::cout << "GET test_key: " << reply->str << std::endl;
        freeReplyObject(reply);
    }

    // DEL
    reply = static_cast<redisReply *>(redisCommand(c, "DEL %s", "test_key"));
    if (reply) {
        std::cout << "DEL test_key: " << reply->integer << std::endl;
        freeReplyObject(reply);
    }

    redisFree(c);
    std::cout << "Redis test done" << std::endl;
}