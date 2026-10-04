#include <iostream>
#include <exception>

#include "../database/Database.h"


int main() {

    try {

        Database database("data/calculator.db");

        database.initialize();


        // 删除 ID = 1
        database.deleteHistory(1);


        std::vector<HistoryRecord> history =
            database.getHistory();


        std::cout << "\nHistory:\n";


        for (const auto& record : history) {

            std::cout
                << "ID: "
                << record.id
                << '\n'

                << "Expression: "
                << record.expression
                << '\n'

                << "Result: "
                << record.result
                << '\n'

                << "Created at: "
                << record.createdAt
                << "\n\n";
        }

    }

    catch (const std::exception& error) {

        std::cout
            << "Database Error: "
            << error.what()
            << '\n';
    }

    return 0;
}