#include <iostream>
#include <string>
#include <exception>

#include "httplib.h"
#include "json.hpp"

#include "Tokenizer.h"
#include "ExpressionParser.h"
#include "Calculator.h"
#include "Database.h"


using json = nlohmann::json;


// ============================================================
// CORS
// ============================================================

void SetCorsHeaders(httplib::Response& response) {
    response.set_header(
        "Access-Control-Allow-Origin",
        "*"
    );

    response.set_header(
        "Access-Control-Allow-Methods",
        "POST, GET, DELETE, OPTIONS"
    );

    response.set_header(
        "Access-Control-Allow-Headers",
        "Content-Type"
    );
}

// ============================================================
// POST /api/calculate
// ============================================================

void PostHandler(
    const httplib::Request& request,
    httplib::Response& response,
    Database& database
) {
    SetCorsHeaders(response);


    // --------------------------------------------------------
    // 1. 检查 JSON 是否合法
    // --------------------------------------------------------

    if (!json::accept(request.body)) {
        response.status = 400;

        response.set_content(
            R"({"success":false,"error":"Invalid JSON"})",
            "application/json"
        );

        return;
    }


    json data = json::parse(request.body);


    // --------------------------------------------------------
    // 2. 检查 expression 字段
    // --------------------------------------------------------

    if (!data.contains("expression")) {
        response.status = 400;

        response.set_content(
            R"({"success":false,"error":"Missing Expression"})",
            "application/json"
        );

        return;
    }


    if (!data["expression"].is_string()) {
        response.status = 400;

        response.set_content(
            R"({"success":false,"error":"Expression must be a string"})",
            "application/json"
        );

        return;
    }


    std::string expression =
        data["expression"].get<std::string>();


    std::cout
        << "[Calculate] "
        << expression
        << '\n';


    // --------------------------------------------------------
    // 3. 表达式计算
    // --------------------------------------------------------

    double result;

    try {
        auto tokens =
            Tokenizer::tokenize(expression);

        auto postfix =
            ExpressionParser::toPostfix(tokens);

        result =
            Calculator::evaluate(postfix);
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

        return;
    }


    // --------------------------------------------------------
    // 4. 保存历史记录
    // --------------------------------------------------------

    try {
        database.insertHistory(
            expression,
            result
        );
    }
    catch (const std::exception& error) {
        json errorJson = {
            {"success", false},
            {"error", "Failed to save calculation history"}
        };

        std::cerr
            << "[Database Error] "
            << error.what()
            << '\n';

        response.status = 500;

        response.set_content(
            errorJson.dump(),
            "application/json"
        );

        return;
    }


    // --------------------------------------------------------
    // 5. 返回结果
    // --------------------------------------------------------

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


// ============================================================
// GET /api/history
// ============================================================

void GetHistoryHandler(
    const httplib::Request& request,
    httplib::Response& response,
    Database& database
) {
    SetCorsHeaders(response);


    try {
        auto history =
            database.getHistory();


        json historyJson =
            json::array();


        for (const auto& record : history) {
            historyJson.push_back({
                {"id", record.id},
                {"expression", record.expression},
                {"result", record.result},
                {"created_at", record.createdAt}
            });
        }


        response.status = 200;

        response.set_content(
            historyJson.dump(),
            "application/json"
        );
    }
    catch (const std::exception& error) {
        json errorJson = {
            {"success", false},
            {"error", error.what()}
        };


        response.status = 500;

        response.set_content(
            errorJson.dump(),
            "application/json"
        );
    }
}


// ============================================================
// DELETE /api/history/{id}
// ============================================================

void DeleteHistoryHandler(
    const httplib::Request& request,
    httplib::Response& response,
    Database& database
) {
    SetCorsHeaders(response);


    try {
        // /api/history/12
        //
        // 正则中的 (\d+) 会被捕获到 matches[1]

        int id =
            std::stoi(
                request.matches[1].str()
            );


        database.deleteHistory(id);


        json resultJson = {
            {"success", true},
            {"deleted_id", id}
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


        response.status = 500;

        response.set_content(
            errorJson.dump(),
            "application/json"
        );
    }
}


// ============================================================
// main
// ============================================================

int main() {

    try {

        // ====================================================
        // Database
        // ====================================================

        Database database(
            "data/calculator.db"
        );

        database.initialize();


        std::cout
            << "Database initialized successfully."
            << '\n';


        // ====================================================
        // HTTP Server
        // ====================================================

        httplib::Server server;


        // ----------------------------------------------------
        // CORS preflight
        // ----------------------------------------------------

        server.Options(
            ".*",
            [](
                const httplib::Request& request,
                httplib::Response& response
            ) {
                SetCorsHeaders(response);

                response.status = 204;
            }
        );


        // ----------------------------------------------------
        // POST /api/calculate
        // ----------------------------------------------------

        server.Post(
            "/api/calculate",

            [&database](
                const httplib::Request& request,
                httplib::Response& response
            ) {
                PostHandler(
                    request,
                    response,
                    database
                );
            }
        );


        // ----------------------------------------------------
        // GET /api/history
        // ----------------------------------------------------

        server.Get(
            "/api/history",

            [&database](
                const httplib::Request& request,
                httplib::Response& response
            ) {
                GetHistoryHandler(
                    request,
                    response,
                    database
                );
            }
        );


        // ----------------------------------------------------
        // DELETE /api/history/{id}
        // ----------------------------------------------------

        server.Delete(
            R"(/api/history/(\d+))",

            [&database](
                const httplib::Request& request,
                httplib::Response& response
            ) {
                DeleteHistoryHandler(
                    request,
                    response,
                    database
                );
            }
        );


        // ====================================================
        // Start server
        // ====================================================

        std::cout
            << "Backend server running at "
            << "http://127.0.0.1:8080"
            << '\n';


        server.listen(
            "127.0.0.1",
            8080
        );
    }

    catch (const std::exception& error) {

        std::cerr
            << "Fatal Error: "
            << error.what()
            << '\n';

        return 1;
    }


    return 0;
}