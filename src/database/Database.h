#pragma once

#include <string>
#include <vector>

#include "sqlite3.h"


struct HistoryRecord {
    int id;

    std::string expression;

    double result;

    std::string createdAt;
};


class Database {
public:
    explicit Database(const std::string& path);

    ~Database();

    void initialize();

    void insertHistory(
        const std::string& expression,
        double result
    );

    std::vector<HistoryRecord> getHistory();

    void deleteHistory(int id);

private:
    sqlite3* db = nullptr;
};