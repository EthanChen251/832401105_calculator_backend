#pragma once

#include <vector>
#include "../parser/Token.h"


class Calculator {
public:
    static double evaluate(
        const std::vector<Token>& postfix
    );
};