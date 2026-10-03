#pragma once

#include <string>

enum class TokenType {
    Number,

    Plus,
    Minus,

    Multiply,
    Divide,

    UnaryPlus,
    UnaryMiners,

    LeftParen,
    RightParen
};

struct Token {
    TokenType type;
    std::string text;
};