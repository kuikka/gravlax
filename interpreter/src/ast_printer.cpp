#include <fmt/core.h>
#include <iostream>

// #include <gravlax/expression.h>
// #include <gravlax/token.h>

// #include <gravlax/generated/binary.h>
// #include <gravlax/generated/grouping.h>
// #include <gravlax/generated/literal.h>
// #include <gravlax/generated/unary.h>

#include <gravlax/ast_printer.h>

namespace gravlax
{
using gravlax::generated::Binary;
using gravlax::generated::Grouping;
using gravlax::generated::Literal;
using gravlax::generated::Unary;

std::string AstPrinter::visitBinaryExpr(Binary<std::string> &expr)
{
    return parenthesize(expr.oper->lexeme, expr.left, expr.right);
}

std::string AstPrinter::visitGroupingExpr(Grouping<std::string> &expr)
{
    return parenthesize("group", expr.expression);
}

std::string AstPrinter::visitLiteralExpr(Literal<std::string> &expr)
{
    return Token::literal_as_string(expr.value);
}

std::string AstPrinter::visitUnaryExpr(Unary<std::string> &expr)
{
    return parenthesize(expr.oper->lexeme, expr.right);
}

std::string AstPrinter::parenthesize(Expr<std::string> &expr)
{
    return expr.accept(*this);
}

std::string AstPrinter::parenthesize(
    std::string name,
    std::convertible_to<std::shared_ptr<Expr<std::string>>> auto &...exprs)
{
    std::string s;

    s += "(";
    s += name;

    for (auto &expr :
         std::initializer_list<std::shared_ptr<Expr<std::string>>>{exprs...}) {
        s += " ";
        s += expr->accept(*this);
    }

    s += ")";

    return s;
}

std::string AstPrinter::print(std::shared_ptr<Expr<std::string>> expr)
{
    return expr->accept(*this);
}

}; // namespace gravlax

#if 0
int main()
{
    AstPrinter printer;

    auto expression = std::make_shared<Binary<std::string>>(
        std::make_shared<Unary<std::string>>(
            std::make_shared<Token>(Token::Type::MINUS, "-", 1.0),
            std::make_shared<Literal<std::string>>(123.0)),
        std::make_shared<Token>(Token::Type::STAR, "*", 1.0),
        std::make_shared<Grouping<std::string>>(
            std::make_shared<Literal<std::string>>(45.67)));

    std::cout << printer.print(expression);

    return 0;
}
#endif