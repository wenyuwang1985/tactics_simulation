add_library(config SHARED
    src/ClientConfig.cpp
)

target_include_directories(config
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(config
    PUBLIC
        ryml        # 自动关联头文件和库
        #c4core     
        log         # 假设你的日志模块目标名
    PRIVATE
        nlohmann_json
)