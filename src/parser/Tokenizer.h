#pragma once

#include <string>
#include <vector>
#include <stdexcept>

#include "Token.h"

class Tokenizer {
public:
    static std::vector<Token> tokenize(const std::string& input);
};

