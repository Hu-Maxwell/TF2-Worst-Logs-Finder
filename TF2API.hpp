#ifndef TF2API_HPP
#define TF2API_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "crow.h"
#include <curl/curl.h>
#include <nlohmann/json.hpp>

std::string read_file(const std::string& filepath);
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* buffer);
std::vector<std::string> getTotalLogs();
std::vector<std::pair<std::string, float>> getTopFiveWorstLogs(std::vector<std::pair<std::string, float>> ratioVectorDPM);
std::vector<std::pair<std::string, float>> getDPM();

#endif