#include "TF2API.hpp"

std::string read_file(const std::string& filepath) {
    std::ifstream file("static/" + filepath);
    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    std::cerr << "File not found: static/" << filepath << std::endl;
    return "File not found.";
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* buffer) {
    size_t newLength = size * nmemb;
    buffer->append((char*)contents, newLength);
    return newLength;
}

std::string convertSteamID64ToSteamID3(const std::string& steamID64) {
    const long long steamIDBase = 76561197960265728LL;
    long long id = std::stoll(steamID64);
    long long accountID = id - steamIDBase;

    return "[U:1:" + std::to_string(accountID) + "]";
}

std::vector<std::string> getTotalLogs() {
    std::vector<std::string> logIDs; 
    const std::string steamID64 = "76561198930130823"; 
    const std::string apiUrl = "https://logs.tf/api/v1/log?player=" + steamID64;

    CURL* curl = curl_easy_init();
    std::string responseString;
 
    curl_global_init(CURL_GLOBAL_DEFAULT);

    float totalLogs = 0;
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);
 
        CURLcode res = curl_easy_perform(curl);
 
        if(res != CURLE_OK) {
            std::cerr << "Failed." << curl_easy_strerror(res) << std::endl;
        } else {
            auto jsonResponse = nlohmann::json::parse(responseString);
            totalLogs = jsonResponse["total"]; 
         }
 
        curl_easy_cleanup(curl);
    }
 
    curl_global_cleanup();
    return logIDs; 
}

std::string getDPM() {
    std::string logID = "3732357";
    std::string playerUID = "[U:1:431667580]"; 
 
    // http://logs.tf/api/v1/log/<log_id> or http://logs.tf/json/<log_id> 
    std::string apiUrl = "https://logs.tf/api/v1/log/" + logID;

    // dewe's UID
    // [U:1:431667580]
 
    CURL* curl;
    CURLcode res;
    std::string responseString;
 
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    std::string DPMString;
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);
 
        res = curl_easy_perform(curl);
 
        if(res != CURLE_OK) {
            std::cerr << "Failed." << curl_easy_strerror(res) << std::endl;
        } else {
            auto jsonResponse = nlohmann::json::parse(responseString);

            float totalLogs = getTotalLogs(); 
            float DPM = jsonResponse["players"][playerUID]["dapm"];
            DPMString = "The DPM you hit in this log was " + std::to_string(DPM) + ". You have " + std::to_string(totalLogs) + " logs played.";
         }
 
        curl_easy_cleanup(curl);
    }
 
    curl_global_cleanup();
    return DPMString; 
}


// to get worst log
// search all the logs of a single person
// get all log IDs 
// float totalLogs = jsonResponse["logs"].size();
// float logs[totalLogs]

// for each log id, 
    // average the DPM for all classes except medic and player. (if type != "medic") 
    // compare player's DPM to average DPM.
    // if it's the lowest so far, store it in lowestRatioDPM

// print lowestRatioDPM. 