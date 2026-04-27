add_library(message_sender SHARED
    src/MessageSender.cpp
)

target_include_directories(message_sender
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# =========================
# Architecture Detection
# =========================
# Detect CPU architecture and select corresponding third_party directory
# Priority: explicit ARCH_DIR > compiler name > CMAKE_SYSTEM_PROCESSOR

if(DEFINED ARCH_DIR)
    # User explicitly specified architecture
    message(STATUS "Architecture explicitly set to: ${ARCH_DIR}")
elseif(CMAKE_C_COMPILER MATCHES "aarch64")
    # Detect from C compiler (cross-compilation)
    set(ARCH_DIR "aarch64")
    message(STATUS "Detected architecture from C compiler: aarch64")
elseif(CMAKE_CXX_COMPILER MATCHES "aarch64")
    # Detect from CXX compiler (cross-compilation)
    set(ARCH_DIR "aarch64")
    message(STATUS "Detected architecture from CXX compiler: aarch64")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64|ARM64")
    set(ARCH_DIR "aarch64")
    message(STATUS "Detected architecture from CMAKE_SYSTEM_PROCESSOR: aarch64")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|x86|AMD64" OR CMAKE_MACHINE MATCHES "x86_64")
    set(ARCH_DIR "x86_64")
    message(STATUS "Detected architecture from CMAKE_SYSTEM_PROCESSOR: x86_64")
else()
    message(WARNING "Unknown architecture: CMAKE_SYSTEM_PROCESSOR=${CMAKE_SYSTEM_PROCESSOR}. Defaulting to x86_64. Use -DARCH_DIR=aarch64 to override.")
    set(ARCH_DIR "x86_64")
endif()

message(STATUS "Using third_party directory: ${CMAKE_SOURCE_DIR}/third_party/${ARCH_DIR}")

# =========================
# Fast DDS IMPORTED targets
# =========================
set(THIRD_PARTY_BASE_DIR
    ${CMAKE_SOURCE_DIR}/third_party/${ARCH_DIR}
)

set(FASTDDS_ROOT
    ${THIRD_PARTY_BASE_DIR}/fast_dds
)

set(FASTCDR_ROOT
    ${THIRD_PARTY_BASE_DIR}/fast_cdr
)

set(FOONATHAN_MEMORY_ROOT
    ${THIRD_PARTY_BASE_DIR}/foonathan_memory_vendor
)

set(FASTDDS_INCLUDE_DIR
    ${FASTDDS_ROOT}/include
)

set(FASTCDR_INCLUDE_DIR
    ${FASTCDR_ROOT}/include
)

set(FOONATHAN_MEMORY_INCLUDE_DIR
    ${FOONATHAN_MEMORY_ROOT}/include
)

set(FASTDDS_LIBRARY
    ${FASTDDS_ROOT}/lib/libfastdds.so
)

set(FASTCDR_LIBRARY
    ${FASTCDR_ROOT}/lib/libfastcdr.so
)

set(FOONATHAN_MEMORY_LIBRARY
    ${FOONATHAN_MEMORY_ROOT}/lib/libfoonathan_memory-0.7.3.so
)

foreach(THIRD_PARTY_LIB
        FASTDDS_LIBRARY
        FASTCDR_LIBRARY
        FOONATHAN_MEMORY_LIBRARY)
    if(NOT EXISTS ${${THIRD_PARTY_LIB}})
        message(FATAL_ERROR "[fastdds] library not found: ${${THIRD_PARTY_LIB}}")
    endif()
endforeach()

find_package(Threads REQUIRED)

if(NOT TARGET fastcdr)
    add_library(fastcdr SHARED IMPORTED GLOBAL)
    set_target_properties(fastcdr PROPERTIES
        IMPORTED_LOCATION ${FASTCDR_LIBRARY}
        INTERFACE_INCLUDE_DIRECTORIES ${FASTCDR_INCLUDE_DIR}
    )
endif()

if(NOT TARGET foonathan_memory)
    add_library(foonathan_memory SHARED IMPORTED GLOBAL)
    set_target_properties(foonathan_memory PROPERTIES
        IMPORTED_LOCATION ${FOONATHAN_MEMORY_LIBRARY}
        INTERFACE_INCLUDE_DIRECTORIES ${FOONATHAN_MEMORY_INCLUDE_DIR}
    )
endif()

if(NOT TARGET fastdds)
    add_library(fastdds SHARED IMPORTED GLOBAL)
    set_target_properties(fastdds PROPERTIES
        IMPORTED_LOCATION ${FASTDDS_LIBRARY}
        INTERFACE_INCLUDE_DIRECTORIES "${FASTDDS_INCLUDE_DIR};${FASTCDR_INCLUDE_DIR}"
    )
endif()

set(FASTDDS_SYSTEM_LIBS
    Threads::Threads
    ${CMAKE_DL_LIBS}
)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    list(APPEND FASTDDS_SYSTEM_LIBS rt)
endif()

set_target_properties(message_sender PROPERTIES
    BUILD_RPATH "${FASTDDS_ROOT}/lib;${FASTCDR_ROOT}/lib;${FOONATHAN_MEMORY_ROOT}/lib"
)

target_link_libraries(message_sender
    PRIVATE
        nlohmann_json
        fastdds
        fastcdr
        foonathan_memory
        ${FASTDDS_SYSTEM_LIBS}
)
