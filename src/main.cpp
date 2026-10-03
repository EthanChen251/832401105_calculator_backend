#include <iostream>
#include <string>
#include "httplib.h"
#include "json.hpp"
#include "Tokenizer.h"
#include "Calculator.h"
#include "ExpressionParser.h"

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

void PostHandler(
    const httplib::Request& request,
    httplib::Response& response
) {
    response.set_header(
        "Access-Control-Allow-Origin",
        "http://127.0.0.1:5500"
    );

    // JSON 检查
    if (!json::accept(request.body)) {
        response.status = 400;
        response.set_content(
            R"({"error":"Invalid JSON"})",
            "application/json"
        );
        return;
    }

    json data = json::parse(request.body);

    if (!data.contains("expression")) {
        response.status = 400;
        response.set_content(
            R"({"error":"Missing Expression"})",
            "application/json"
        );
        return;
    }

    if (!data["expression"].is_string()) {
        response.status = 400;
        response.set_content(
            R"({"error":"Expression must be a string"})",
            "application/json"
        );
        return;
    }

    std::string expression =
        data["expression"].get<std::string>();


    // ============================
    // 真正的表达式计算
    // ============================
    try {
        auto tokens =
            Tokenizer::tokenize(expression);

        auto postfix =
            ExpressionParser::toPostfix(tokens);

        double result =
            Calculator::evaluate(postfix);


        json resultJson = {
            {"success", true},
            {"result", result}
        };

        response.status = 200;
        response.set_content(
            resultJson.dump(),
            "application/json"
        );
    }

    catch (const std::exception& error) {

        json errorJson = {
            {"success", false},
            {"error", error.what()}
        };

        response.status = 400;
        response.set_content(
            errorJson.dump(),
            "application/json"
        );
    }
}