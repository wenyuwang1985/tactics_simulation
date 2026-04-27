add_library(result_receiver SHARED
    src/ResultReceiver.cpp
    src/TestEvent.cpp
)

target_include_directories(result_receiver
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(result_receiver
    PUBLIC
        config
        
        log
    PRIVATE
        nlohmann_json
        hiredis::hiredis
        event_static
)