# cmake/third_party/hiredis.cmake
# hiredis: 强制静态库，并用 CACHE INTERNAL 防止其 OPTION 覆盖全局值

set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "" FORCE)
set(DISABLE_TESTS ON CACHE INTERNAL "" FORCE)
set(ENABLE_EXAMPLES OFF CACHE INTERNAL "" FORCE)
set(ENABLE_NUGET OFF CACHE INTERNAL "" FORCE)

if (NOT TARGET hiredis::hiredis)
    add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/hiredis EXCLUDE_FROM_ALL)
endif()
