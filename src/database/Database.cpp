#include "Database.h"

#include <stdexcept>


Database::Database(const std::string& path) {

    int result = sqlite3_open(
        path.c_str(),
        &db
    );

    if (result != SQLITE_OK) {

        std::string message =
            sqlite3_errmsg(db);

        sqlite3_close(db);

        db = nullptr;

        throw std::runtime_error(
            "Failed to open database: " + message
        );
    }
}


Database::~Database() {

    if (db != nullptr) {
        sqlite3_close(db);
    }
}


void Database::initialize() {

    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS calculation_history (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            expression TEXT NOT NULL,
            result REAL NOT NULL,
            created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP
        );
    )";


    char* errorMessage = nullptr;


    int result = sqlite3_exec(
        db,
        sql,
        nullptr,
        nullptr,
        &errorMessage
    );


    if (result != SQLITE_OK) {

        std::string message =
            errorMessage != nullptr
            ? errorMessage
            : "Unknown SQLite error";

        sqlite3_free(errorMessage);

        throw std::runtime_error(
            "Failed to initialize database: "
            + message
        );
    }
}

void Database::insertHistory(
    const std::string& expression,
    double result
) {
    const char* sql = R"(
        INSERT INTO calculation_history (
            expression,
            result
        )
        VALUES (?, ?);
    )";

    sqlite3_stmt* statement = nullptr;


    // 1. 准备 SQL
    int prepareResult = sqlite3_prepare_v2(
        db,
        sql,
        -1,
        &statement,
        nullptr
    );

    if (prepareResult != SQLITE_OK) {
        throw std::runtime_error(
            "Failed to prepare insert statement: "
            + std::string(sqlite3_errmsg(db))
        );
    }


    // 2. 给 ? 绑定真正的数据
    sqlite3_bind_text(
        statement,
        1,
        expression.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_double(
        statement,
        2,
        result
    );


    // 3. 执行
    int stepResult = sqlite3_step(statement);

    if (stepResult != SQLITE_DONE) {

        std::string message =
            sqlite3_errmsg(db);

        sqlite3_finalize(statement);

        throw std::runtime_error(
            "Failed to insert history: " + message
        );
    }


    // 4. 释放 statement
    sqlite3_finalize(statement);
}

std::vector<HistoryRecord> Database::getHistory() {

    const char* sql = R"(
        SELECT
            id,
            expression,
            result,
            created_at
        FROM calculation_history
        ORDER BY id DESC;
    )";


    sqlite3_stmt* statement = nullptr;


    int prepareResult = sqlite3_prepare_v2(
        db,
        sql,
        -1,
        &statement,
        nullptr
    );


    if (prepareResult != SQLITE_OK) {

        throw std::runtime_error(
            "Failed to prepare select statement: "
            + std::string(sqlite3_errmsg(db))
        );
    }


    std::vector<HistoryRecord> history;


    while (sqlite3_step(statement) == SQLITE_ROW) {

        HistoryRecord record;


        record.id =
            sqlite3_column_int(
                statement,
                0
            );


        record.expression =
            reinterpret_cast<const char*>(
                sqlite3_column_text(
                    statement,
                    1
                )
            );


        record.result =
            sqlite3_column_double(
                statement,
                2
            );


        record.createdAt =
            reinterpret_cast<const char*>(
                sqlite3_column_text(
                    statement,
                    3
                )
            );


        history.push_back(record);
    }


    sqlite3_finalize(statement);


    return history;
}

void Database::deleteHistory(int id) {

    const char* sql = R"(
        DELETE FROM calculation_history
        WHERE id = ?;
    )";

    sqlite3_stmt* statement = nullptr;


    int prepareResult = sqlite3_prepare_v2(
        db,
        sql,
        -1,
        &statement,
        nullptr
    );

    if (prepareResult != SQLITE_OK) {
        throw std::runtime_error(
            "Failed to prepare delete statement: "
            + std::string(sqlite3_errmsg(db))
        );
    }


    // 第 1 个 ? 绑定 id
    sqlite3_bind_int(
        statement,
        1,
        id
    );


    int stepResult =
        sqlite3_step(statement);


    if (stepResult != SQLITE_DONE) {

        std::string message =
            sqlite3_errmsg(db);

        sqlite3_finalize(statement);

        throw std::runtime_error(
            "Failed to delete history: "
            + message
        );
    }


    sqlite3_finalize(statement);
}