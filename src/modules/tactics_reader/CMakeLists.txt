add_library(tactics_reader SHARED
    src/TacticsReader.cpp
    src/Entity.cpp
)

target_include_directories(tactics_reader
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(tactics_reader
    PUBLIC
        nlohmann_json
    PRIVATE
        log
)
