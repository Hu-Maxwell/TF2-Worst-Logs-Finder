cmake_minimum_required(VERSION 3.5.0)
project(TF2-Worst-Logs VERSION 0.1.0 LANGUAGES C CXX)

find_package(Crow REQUIRED)

add_executable(TF2-Worst-Logs main.cpp)

target_link_libraries(TF2-Worst-Logs Crow::Crow)


