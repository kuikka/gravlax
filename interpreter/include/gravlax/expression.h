#pragma once

#include <gravlax/token.h>

#include <gravlax/generated/visitor_base.h>

namespace gravlax
{

template <typename R> struct Expr {
    virtual R accept(gravlax::generated::ExprVisitorBase<R> &visitor) = 0;
};

}; // namespace gravlax
