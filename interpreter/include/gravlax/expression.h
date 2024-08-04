#pragma once

#include <gravlax/token.h>

namespace gravlax
{

struct Expression {
    struct Binary {
        std::shared_ptr<Expression> left;
        Token oper;
        std::shared_ptr<Expression> right;

        Binary(std::shared_ptr<Expression> left, Token oper,
               std::shared_ptr<Expression> right)
            : left(left), oper(oper), right(right)
        {
        }
    };
};

}; // namespace gravlax
