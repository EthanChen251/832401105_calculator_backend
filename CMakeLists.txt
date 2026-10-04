cmake_minimum_required(VERSION 3.16)

project(calculator_backend LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(calculator_backend
    src/main.cpp

    src/parser/Tokenizer.cpp
    src/parser/ExpressionParser.cpp

    src/calculator/Calculator.cpp
    third_party/sqlite/sqlite3.c
    src/database/Database.cpp
)

target_include_directories(calculator_backend PRIVATE
    ${CMAKE_SOURCE_DIR}/third_party
    ${CMAKE_SOURCE_DIR}/third_party/sqlite
    ${CMAKE_SOURCE_DIR}/src/parser
    ${CMAKE_SOURCE_DIR}/src/calculator
    ${CMAKE_SOURCE_DIR}/src/database

)

if(WIN32)
    target_compile_definitions(calculator_backend PRIVATE
        _WIN32_WINNT=0x0A00
        WINVER=0x0A00
    )
endif()

find_package(Threads REQUIRED)

target_link_libraries(calculator_backend PRIVATE
    Threads::Threads
)

if(WIN32)
    target_link_libraries(calculator_backend PRIVATE
        ws2_32
        crypt32
    )
endif()

add_executable(database_test
    src/test/database_test.cpp
    src/database/Database.cpp
    third_party/sqlite/sqlite3.c
)

target_include_directories(database_test PRIVATE
    ${CMAKE_SOURCE_DIR}/third_party/sqlite
    ${CMAKE_SOURCE_DIR}/src/database
)