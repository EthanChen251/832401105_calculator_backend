#include "Calculator.h"

#include <stack>
#include <stdexcept>
#include <string>


double Calculator::evaluate(
    const std::vector<Token>& postfix
) {
    std::stack<double> numbers;

    if (postfix.empty()) {
        throw std::runtime_error("Empty postfix expression");
    }

    for (const auto& token : postfix) {

        switch (token.type) {

        // ========================================
        // Number
        // ========================================
        case TokenKind::Number: {

            // token.text 现在还是字符串
            //
            // 例如：
            // "123.45"
            //
            // TODO:
            // 把它转换成 double
            double value = std::stod(token.text);
            numbers.push(value);

            break;
        }


        // ========================================
        // Operators
        // ========================================
        case TokenKind::UnaryMinus:
        case TokenKind::UnaryPlus:
            if (numbers.empty()) {
                throw std::runtime_error("Invalid Unary Expression");
            }
            else {
                double result;
                if (token.type == TokenKind::UnaryMinus) {
                    result = -numbers.top();
                }
                if (token.type == TokenKind::UnaryPlus) {
                    result = numbers.top();
                }
                numbers.pop();
                numbers.push(result);
            }
            break;


        case TokenKind::Plus:
        case TokenKind::Minus:
        case TokenKind::Multiply:
        case TokenKind::Divide: {

            // 一个二元运算符必须需要两个操作数
            //
            // TODO:
            // 如果 numbers 里面不足两个数字，
            // 应该抛出异常
            if (numbers.size() < 2) {
                throw std::runtime_error("计算栈小于两个数");
            }

            // 注意顺序！
            //
            // 例如：
            // 8 2 /
            //
            // 先 pop 出来的是 2
            // 后 pop 出来的是 8

            double right = numbers.top();
            numbers.pop();
            double left = numbers.top();
            numbers.pop();

            double result = 0.0;

            switch (token.type) {

            case TokenKind::Plus:
                result = left + right;
                break;

            case TokenKind::Minus:
                result = left - right;
                break;

            case TokenKind::Multiply:
                result = left * right;
                break;

            case TokenKind::Divide:
                if (right == 0) {
                    throw std::runtime_error("Cannot divide by zero!");
                }
                else {
                    result = left / right;
                }
                break;

            default:
                break;
            }


            // TODO:
            // 把计算结果重新压回 numbers
            numbers.push(result);

            break;
        }


        // ========================================
        // 不应该出现在 postfix 中的 Token
        // ========================================
        default:
            // postfix 中理论上不应该再有：
            // (
            // )
            //
            // 如果出现了，说明 Parser 有问题
            throw std::runtime_error(
                "Invalid token in postfix expression"
            );
        }
    }


    // ========================================
    // 最终结果检查
    // ========================================
    // 正常计算结束以后，
    // numbers 里面应该刚好剩一个数字
    //
    // TODO:
    // 如果不是 1 个，说明表达式有问题
    if (numbers.size() != 1) {
        throw std::runtime_error("Expression Wrong!");
    }

    // TODO:
    // 返回最后结果
    return numbers.top();
}