#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

#include "../parser/Tokenizer.h"
#include "../parser/ExpressionParser.h"
#include "../calculator/Calculator.h"


int main() {
    std::string expression;

    std::cout << "Enter expression: ";
    std::getline(std::cin, expression);

    try {
        // 1. 原始字符串 -> Tokens
        std::vector<Token> tokens =
            Tokenizer::tokenize(expression);

        // 2. Tokens -> Postfix
        std::vector<Token> postfix =
            ExpressionParser::toPostfix(tokens);

        // 输出 postfix，方便检查
        std::cout << "\nPostfix:\n";

        for (const auto& token : postfix) {
            std::cout << token.text << ' ';
        }

        std::cout << '\n';

        // 3. Postfix -> Result
        double result =
            Calculator::evaluate(postfix);

        std::cout << "\nResult: "
                  << result
                  << '\n';
    }

    catch (const std::exception& error) {
        std::cout
            << "\nError: "
            << error.what()
            << '\n';
    }

    return 0;
}