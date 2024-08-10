#pragma once

#include <string>

#include <gravlax/expression.h>

#include <gravlax/generated/binary.h>
#include <gravlax/generated/grouping.h>
#include <gravlax/generated/literal.h>
#include <gravlax/generated/unary.h>

namespace gravlax
{

class AstPrinter : public gravlax::generated::ExprVisitorBase<std::string>
{
  public:
    virtual std::string
    visitBinaryExpr(gravlax::generated::Binary<std::string> &expr) override;
    virtual std::string
    visitGroupingExpr(gravlax::generated::Grouping<std::string> &expr) override;
    virtual std::string
    visitLiteralExpr(gravlax::generated::Literal<std::string> &expr) override;
    virtual std::string
    visitUnaryExpr(gravlax::generated::Unary<std::string> &expr) override;

    std::string parenthesize(Expr<std::string> &expr);

    std::string
    parenthesize(std::string name,
                 auto ...exprs);

    std::string print(Expr<std::string>& expr);
};

}; // namespace gravlax