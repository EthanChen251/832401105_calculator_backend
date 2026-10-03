#include <iostream>
#include <string>
#include "httplib.h"
#include "json.hpp"

using json = nlohmann::json;

void PostHandler(const httplib::Request& request, httplib::Response& response);

int main(void) {
    httplib::Server server;
    server.Options(".*",
    [](const httplib::Request& request,
       httplib::Response& response) {

        response.set_header(
            "Access-Control-Allow-Origin",
            "http://127.0.0.1:5500"
        );

        response.set_header(
            "Access-Control-Allow-Methods",
            "POST, GET, DELETE, OPTIONS"
        );

        response.set_header(
            "Access-Control-Allow-Headers",
            "Content-Type"
        );

        response.status = 204;
    });
    server.Post("/api/calculate", PostHandler);

    server.Get("/api/hello", 
        [](const httplib::Request& request,
                 httplib::Response& response
        ) {
            response.set_content(
                "Hello World",
                "application/json"
            );
        }
    );

    std::cout << "Server running at http://127.0.0.1:8080\n";

    server.listen("127.0.0.1", 8080);

    return 0;
}

void PostHandler(const httplib::Request& request, httplib::Response& response) {
    response.set_header(
    "Access-Control-Allow-Origin",
    "http://127.0.0.1:5500"
    );
    // Check whether the body is valid JSON
    if (!json::accept(request.body)) {
        response.status = 400;
        response.set_content(
            R"({"error":"Invalid JSON"})",
            "application/json"
        );
        std::cout << "Input:\n" << request.body << std::endl << "400 Error: Invalid JSON\n";
        return;
    }

    json data = json::parse(request.body);
    // Check whether the data contain expression
    if (!(data.contains("expression"))) {
        response.status = 400;
        response.set_content(
            R"({"error":"Missing Expression"})",
            "application/json"
        );
        std::cout << "Input:\n" << data << std::endl << "400 Error: Missing Expression\n";
        return;
    }
    if (!(data["expression"].is_string())) {
        response.status = 400;
        response.set_content(
            R"({"error":"Expression must be a string"})",
            "application/json"
        );
        std::cout << "Input:\n" << data << std::endl << "400 Error: Expression must be a string\n";
        return;
    }
    // 给Tokenizer做进一步处理
    
    response.set_content(
        R"({"result":0})",
        "application/json"
    );
    std::cout << "Expression: " << data["expression"] << std::endl;
}