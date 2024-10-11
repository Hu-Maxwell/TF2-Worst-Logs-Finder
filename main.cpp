#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "crow.h"
#include <curl/curl.h>
#include <nlohmann/json.hpp>

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

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* buffer) {
    size_t newLength = size * nmemb;
    buffer->append((char*)contents, newLength);
    return newLength;
}


int main() {
    crow::SimpleApp app;

    // Route the index.html file
    CROW_ROUTE(app, "/")([](){
        crow::response res(read_file("index.html"));
        res.add_header("Content-Type", "text/html");
        return res;
    });

    // Route the styles.css file
    CROW_ROUTE(app, "/styles.css")([](){
        crow::response res(read_file("styles.css"));
        res.add_header("Content-Type", "text/css");
        return res;
    });

    // Route the TF2 logo image
    CROW_ROUTE(app, "/tf2-logo.png")([](){
        crow::response res(read_file("tf2-logo.png"));
        res.add_header("Content-Type", "image/png");
        return res;
    });

    // Route the custom font
    CROW_ROUTE(app, "/tf2build.ttf")([](){
        crow::response res(read_file("tf2build.ttf"));
        res.add_header("Content-Type", "font/ttf");
        return res;
    });

    std::string logID = "3732357";
    std::string playerUID = "[U:1:431667580]"; 
 
    // http://logs.tf/api/v1/log?title=X&uploader=Y&player=Z&limit=N&offset=N 
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
 
            float DPM = jsonResponse["players"][playerUID]["dapm"];
            DPMString = "The DPM you hit in this log was " + std::to_string(DPM) + ".";
         }
 
        curl_easy_cleanup(curl);
    }
 
    curl_global_cleanup();

    CROW_ROUTE(app, "/api/helloworld")([&DPMString](){
        return crow::response(DPMString.c_str());
    });

    // Start the Crow server on port 8080
    app.port(8080).multithreaded().run();
}