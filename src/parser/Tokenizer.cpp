#include "Tokenizer.h"

std::vector<Token> Tokenizer::tokenize(const std::string& input) {
    std::vector<Token> tokens;
    std::size_t i = 0;

    while (i < input.size()) {
        char current = input[i];
        Token temp;

        switch (current)
        {
        case ' ':
            i++;
            continue;

        case '+':
            temp.text = "+";
            temp.type = TokenType::Plus;
            break;
        case '-':
            temp.text = "-";
            temp.type = TokenType::Minus;
            break;
        case '*':
            temp.text = "*";
            temp.type = TokenType::Multiply;
            break;
        case '/':
            temp.text = "/";
            temp.type = TokenType::Divide;
            break;
        case '(':
            temp.text = "(";
            temp.type = TokenType::LeftParen;
            break;
        case ')':
            temp.text = ")";
            temp.type = TokenType::RightParen;
            break;
        default:
            if (current >= '0' && current <= '9') {
                std::string numberText = "";
                bool dotSeen = false;

                while (i < input.size()) {
                    current = input[i];
                    // Number
                    if (current >= '0' && current <= '9') {
                        numberText.push_back(current);
                        i++;
                        continue;
                    }
                    // Dot
                    if (current == '.') {
                        if (!dotSeen) {
                            dotSeen = true;
                            numberText.push_back(current);
                            i++;
                            continue;
                        }
                        else {
                            // Invalid Number
                            throw std::runtime_error("Invalid Number");
                        }
                    }
                    // Others (Number Ends)
                    break;
                }
                // 不管是因为遇到运算符退出
                // 还是因为已经到达字符串结尾
                // 都在这里统一生成 Number Token
                temp.text = numberText;
                temp.type = TokenType::Number;
                tokens.push_back(temp);
                continue;
            }
            else {
                // Invalid Text
                throw std::runtime_error("Invalid Character");
            }
            break;
        }
        tokens.push_back(temp);
        i++;
    }
    return tokens;
}