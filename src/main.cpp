#include <iostream>
#include "httplib.h"
#include "json.hpp"

int main(void) {
    httplib::Server server;

    server.Get("/hello",
        [](const httplib::Request& request,
           httplib::Response& response) {

            response.set_content(
                R"({"message":"Hello"})",
                "application/json"
            );
        }
    );
    
    server.Post("/api/calculate",
        [](const httplib::Request& request,
           httplib::Response& response) {

            std::cout << "This is Request Body:\n" << request.body;

            response.set_content(
                request.body,
                "application/json"
            ); 
        });

    std::cout << "Server running at http://127.0.0.1:8080\n";

    server.listen("127.0.0.1", 8080);

    return 0;
}