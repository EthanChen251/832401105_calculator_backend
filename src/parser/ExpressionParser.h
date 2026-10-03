#pragma once

#include <vector>
#include "Token.h"

class ExpressionParser {
public:
    static std::vector<Token> toPostfix(
        const std::vector<Token>& tokens
    );
};