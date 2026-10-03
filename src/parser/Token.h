#pragma once

#include <string>

enum class TokenKind {
    Number,

    Plus,
    Minus,

    Multiply,
    Divide,

    UnaryPlus,
    UnaryMinus,

    LeftParen,
    RightParen
};

struct Token {
    TokenKind type;
    std::string text;
};