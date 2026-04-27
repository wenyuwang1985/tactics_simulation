# cmake/third_party/rapidyaml.cmake

# 1. 设置 RapidYAML 的构建选项（按需调整）
# 我们通常关闭测试和示例以加快编译速度
set(RYML_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(RYML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(RYML_WITH_TAB_TOKENS ON CACHE BOOL "" FORCE) # 建议开启，对 YAML 支持更好

# 2. 引入子目录
# 这会创建 ryml 目标
if (NOT TARGET ryml)
    add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/rapidyaml EXCLUDE_FROM_ALL)
endif()

# 3. 如果你需要像 spdlog 那样使用命名空间风格，可以创建一个别名（可选）
# add_library(ryml::ryml ALIAS ryml)