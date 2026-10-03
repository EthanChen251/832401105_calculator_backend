#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

#include "../parser/Tokenizer.h"
#include "../parser/ExpressionParser.h"


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


int main() {
    std::string expression;

    std::cout << "Enter expression: ";
    std::getline(std::cin, expression);

    try {
        // 1. Tokenizer
        std::vector<Token> tokens =
            Tokenizer::tokenize(expression);

        std::cout << "\nTokens:\n";

        for (const auto& token : tokens) {
            std::cout
                << tokenTypeToString(token.type)
                << " : "
                << token.text
                << '\n';
        }


        // 2. Parser
        std::vector<Token> postfix =
            ExpressionParser::toPostfix(tokens);

        std::cout << "\nPostfix:\n";

        for (const auto& token : postfix) {
            std::cout << token.text << ' ';
        }

        std::cout << '\n';
    }

    catch (const std::runtime_error& error) {
        std::cout
            << "\nParser Error: "
            << error.what()
            << '\n';
    }

    return 0;
}
