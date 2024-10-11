#include <iostream>
#include "crow.h"
#include "TF2API.hpp"

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

    std::string DPMString;
    CROW_ROUTE(app, "/api/DPM")([&DPMString](){
        DPMString = getDPM();
        return crow::response(DPMString.c_str());
    });

    // Start the Crow server on port 8080
    app.port(8080).multithreaded().run();
}