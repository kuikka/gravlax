#include <fmt/core.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <gravlax/ast_printer.h>

using ::testing::_;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::MockFunction;

using gravlax::Expr;
using gravlax::Token;
using gravlax::generated::Binary;
using gravlax::generated::Grouping;
using gravlax::generated::Literal;
using gravlax::generated::Unary;

class AstPrinterTest : public ::testing::Test
{
};

TEST_F(AstPrinterTest, test_ast_1)
{
    gravlax::AstPrinter printer;

    auto expression = std::make_shared<Binary<std::string>>(
        std::make_shared<Unary<std::string>>(
            std::make_shared<Token>(Token::Type::MINUS, "-", 1.0),
            std::make_shared<Literal<std::string>>(123.0)),
        std::make_shared<Token>(Token::Type::STAR, "*", 1.0),
        std::make_shared<Grouping<std::string>>(
            std::make_shared<Literal<std::string>>(45.67)));

    EXPECT_EQ("(* (- 123.000000) (group 45.670000))",
              printer.print(expression));
}