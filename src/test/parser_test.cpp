#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

#include "../parser/Tokenizer.h"
#include "../parser/ExpressionParser.h"


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
                << TokenKindToString(token.type)
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
