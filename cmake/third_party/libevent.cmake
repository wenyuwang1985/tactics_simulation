# cmake/third_party/libevent.cmake
# libevent 配置：关闭 OpenSSL 依赖和各种无用的测试、示例，只编译静态库

set(EVENT__DISABLE_OPENSSL ON CACHE INTERNAL "Disable OpenSSL" FORCE)
set(EVENT__DISABLE_TESTS ON CACHE INTERNAL "Disable tests" FORCE)
set(EVENT__DISABLE_SAMPLES ON CACHE INTERNAL "Disable samples" FORCE)
set(EVENT__DISABLE_REGRESS ON CACHE INTERNAL "Disable regress" FORCE)
set(EVENT__DISABLE_BENCHMARK ON CACHE INTERNAL "Disable benchmark" FORCE)
set(EVENT__LIBRARY_TYPE STATIC CACHE INTERNAL "Build static libs only" FORCE)

if (NOT TARGET event_static)
    add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/libevent EXCLUDE_FROM_ALL)
endif()
