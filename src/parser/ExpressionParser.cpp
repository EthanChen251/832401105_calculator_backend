#include "ExpressionParser.h"

#include <stack>
#include <stdexcept>


enum class ParseState {
    ExpectOperand,
    ExpectOperator
};


static int precedence(Token token) {
    switch (token.type) {

    case TokenType::Plus:
    case TokenType::Minus:
        return 1;

    case TokenType::Multiply:
    case TokenType::Divide:
        return 2;

    default:
        return 0;
    }
}


std::vector<Token> ExpressionParser::toPostfix(
    const std::vector<Token>& tokens
) {
    std::vector<Token> postfix;
    std::stack<Token> operators;

    if (tokens.empty()) {
        throw std::runtime_error("Empty expression");
    }

    ParseState state = ParseState::ExpectOperand;

    // 当前尚未匹配的左括号数量
    int parenDepth = 0;


    for (auto token : tokens) {

        switch (state) {

        // ========================================
        // 当前期待 Operand
        // ========================================
        case ParseState::ExpectOperand:

            switch (token.type) {

            case TokenType::Number:
                // Number 直接进入后缀表达式
                postfix.push_back(token);

                // 接下来期待运算符
                state = ParseState::ExpectOperator;
                break;


            case TokenType::LeftParen:
                // 左括号进入运算符栈
                operators.push(token);
                parenDepth++;

                // 左括号后仍然期待 Operand
                break;


            case TokenType::Plus:
            case TokenType::Minus:
                // 暂时还没有实现一元 +/-。
                throw std::runtime_error(
                    "Unary operator is not implemented yet"
                );


            default:
                // *, /, ) 在这里都非法
                throw std::runtime_error("Syntax Error");
            }

            break;


        // ========================================
        // 当前期待 Operator
        // ========================================
        case ParseState::ExpectOperator:

            switch (token.type) {

            case TokenType::Plus:
            case TokenType::Minus:
            case TokenType::Multiply:
            case TokenType::Divide:

                // 运算符后下一步需要 Operand
                state = ParseState::ExpectOperand;

                // 如果栈顶运算符优先级 >= 当前运算符，
                // 就应该先计算栈顶，所以将它放进 postfix。
                while (
                    !operators.empty() &&
                    operators.top().type != TokenType::LeftParen &&
                    precedence(operators.top()) >= precedence(token)
                ) {
                    postfix.push_back(operators.top());
                    operators.pop();
                }

                // 当前运算符进入栈
                operators.push(token);

                // 非常重要！
                break;


            case TokenType::RightParen:

                if (parenDepth == 0) {
                    throw std::runtime_error(
                        "Unmatched right parenthesis"
                    );
                }

                // 把 '(' 之后的运算符全部送进 postfix
                while (
                    !operators.empty() &&
                    operators.top().type != TokenType::LeftParen
                ) {
                    postfix.push_back(operators.top());
                    operators.pop();
                }

                // 丢掉 '(' 本身
                operators.pop();

                parenDepth--;

                // 整个 (...) 相当于一个完整 Operand，
                // 所以仍然处于 ExpectOperator
                break;


            default:
                // Number 或 '(' 在这里非法
                //
                // 例如：
                // 1 2
                // 1 (2 + 3)
                throw std::runtime_error("Syntax Error");
            }

            break;
        }
    }


    // ========================================
    // 扫描结束后的语法检查
    // ========================================

    // 如果仍然期待 Operand，说明表达式没写完
    //
    // 例如：
    // 1 +
    // 3 *
    // (1 + 2) /
    if (state != ParseState::ExpectOperator) {
        throw std::runtime_error(
            "Expression ends unexpectedly"
        );
    }


    // 所有左括号必须已经匹配
    if (parenDepth != 0) {
        throw std::runtime_error(
            "Unmatched left parenthesis"
        );
    }


    // ========================================
    // 将剩余运算符全部送入 postfix
    // ========================================

    while (!operators.empty()) {
        postfix.push_back(operators.top());
        operators.pop();
    }


    return postfix;
}