cmake_minimum_required(VERSION 3.10)
project(TF2 VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(CURL REQUIRED)
find_package(Crow REQUIRED)

find_path(JSON_INCLUDE_DIR "nlohmann/json.hpp")

add_executable(TF2 main.cpp tf2api.cpp)

target_include_directories(TF2 PRIVATE ${JSON_INCLUDE_DIR})

target_link_libraries(TF2 PRIVATE CURL::libcurl Crow::Crow)
