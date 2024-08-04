#pragma once

#include <gravlax/token.h>

namespace gravlax
{

struct Expr {
    void accept(ExprVisitorBase &visitor) = 0;
};

}; // namespace gravlax
