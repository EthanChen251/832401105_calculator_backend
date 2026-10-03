#include <iostream>
#include <string>
#include <vector>

#include "../parser/Tokenizer.h"


std::string tokenTypeToString(TokenType type) {
    switch (type) {
    case TokenType::Number:
        return "Number";

    case TokenType::Plus:
        return "Plus";

    case TokenType::Minus:
        return "Minus";

    case TokenType::Multiply:
        return "Multiply";

    case TokenType::Divide:
        return "Divide";

    case TokenType::LeftParen:
        return "LeftParen";

    case TokenType::RightParen:
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
            << tokenTypeToString(token.type)
            << " : "
            << token.text
            << '\n';
    }

    return 0;
}