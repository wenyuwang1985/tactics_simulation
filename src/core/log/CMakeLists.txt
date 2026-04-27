add_library(log SHARED
    src/Log.cpp
)

target_include_directories(log
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(log
    PUBLIC
        spdlog::spdlog
)

target_compile_definitions(log
    PUBLIC SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE
)
