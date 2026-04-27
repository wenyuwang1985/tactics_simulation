cmake_minimum_required(VERSION 3.10)

project(ClientSolution LANGUAGES CXX)

include(cmake/common.cmake)

# === 引入第三方单头库声明 ===
include(cmake/third_party/rapidyaml.cmake)
include(cmake/third_party/nlohmann_json.cmake)
include(cmake/third_party/hiredis.cmake)
include(cmake/third_party/libevent.cmake)

if (NOT TARGET spdlog::spdlog)
    add_subdirectory(third_party/spdlog)
endif()

# ---------- app ----------
add_subdirectory(src/app)

# ---------- core ----------
add_subdirectory(src/core)

# ---------- modules ----------
add_subdirectory(src/modules)



