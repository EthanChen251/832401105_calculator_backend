#include <iostream>
#include <string>
#include <vector>

#include "../parser/Tokenizer.h"


std::string TokenKindToString(TokenKind type) {
    switch (type) {
    case TokenKind::Number:
        return "Number";

    case TokenKind::Plus:
        return "Plus";

    case TokenKind::Minus:
        return "Minus";

    case TokenKind::Multiply:
        return "Multiply";

    case TokenKind::Divide:
        return "Divide";

    case TokenKind::LeftParen:
        return "LeftParen";

    case TokenKind::RightParen:
        return "RightParen";

    default:
        return "Unknown";
    }
}


int main(void) {
    std::string expression;

    std::cout << "Enter expression: ";
    std::getline(std::cin, expression);

    std::vector<Token> tokens =
        Tokenizer::tokenize(expression);

    std::cout << "\nTokens:\n";

    for (const Token& token : tokens) {
        std::cout
            << TokenKindToString(token.type)
            << " : "
            << token.text
            << '\n';
    }

    return 0;
}