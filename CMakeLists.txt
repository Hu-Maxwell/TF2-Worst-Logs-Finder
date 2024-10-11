cmake_minimum_required(VERSION 3.10)
project(TF2 VERSION 0.1.0 LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find required packages
find_package(CURL REQUIRED)
find_package(Crow REQUIRED)

# Include nlohmann JSON (header-only, so no linking needed)
find_path(JSON_INCLUDE_DIR "nlohmann/json.hpp")

# Add the executable
add_executable(TF2 main.cpp)

# Include directories for nlohmann JSON
target_include_directories(TF2 PRIVATE ${JSON_INCLUDE_DIR})

# Link required libraries: libcurl and Crow
target_link_libraries(TF2 PRIVATE CURL::libcurl Crow::Crow)
