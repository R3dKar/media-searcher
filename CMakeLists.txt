cmake_minimum_required(VERSION 3.30)
project(media_searcher LANGUAGES CXX)

include(cmake/AddGitSubmodule.cmake)
add_git_submodule(libs/argparse)
add_git_submodule(libs/cpp-httplib)
add_git_submodule(libs/nlohmann-json)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} PRIVATE argparse::argparse httplib::httplib nlohmann_json::nlohmann_json)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
