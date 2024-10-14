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

// todo: change it so that it returns an array of all the log ids 
std::vector<int> getTotalLogs() {
    // playerID is NOT playerUID
    std::vector<int> logIDs; 
    std::string playerID = "76561198391933308"; 

    // http://logs.tf/api/v1/log?title=X&uploader=Y&player=Z&limit=N&offset=N 
    // should be limited if not using all of it! else it will be slow.  
    std::string apiUrl = "https://logs.tf/api/v1/log?player=" + playerID;

    CURL* curl;
    CURLcode res;
    std::string responseString;
 
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    int totalLogs = 0;
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);
 
        res = curl_easy_perform(curl);
 
        if(res != CURLE_OK) {
            std::cerr << "Failed." << curl_easy_strerror(res) << std::endl;
        } else {
            auto jsonResponse = nlohmann::json::parse(responseString);
            totalLogs = jsonResponse["total"]; 

            for(int i = 0; i < totalLogs - 1; i++) {
                logIDs.push_back(jsonResponse["logs"][i]["id"]);
            }
         }
 
        curl_easy_cleanup(curl);
    }
 
    curl_global_cleanup();
    return logIDs; 
}

std::string getDPM() {
    std::string playerUID = "[U:1:431667580]"; 
    std::vector<int> logIDs = getTotalLogs();

    std::string DPMresults = ""; 

    for(int i = 0; i < 10; i++) {        
        CURL* curl = curl_easy_init();  
        if (!curl) {
            std::cerr << "Failed." << std::endl;
            continue;  // skip failed log
        }

        std::string responseString;
        CURLcode res;

        int logID = logIDs[i];
        std::string apiUrl = "https://logs.tf/api/v1/log/" + std::to_string(logID);

        curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);
    
        res = curl_easy_perform(curl);
    
        if(res != CURLE_OK) {
            std::cerr << "Failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            auto jsonResponse = nlohmann::json::parse(responseString);
            int DPM = jsonResponse["players"][playerUID]["dapm"];
                
            DPMresults = DPMresults + "Log ID " + std::to_string(logID) + ": DPM is " + std::to_string(DPM) + "\n";
        }
    
        curl_easy_cleanup(curl);
    }

    return DPMresults; 
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