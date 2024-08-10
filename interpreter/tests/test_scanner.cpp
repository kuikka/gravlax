#include <fmt/core.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <gravlax/scanner.h>

using ::gravlax::Token;
using ::testing::_;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::MockFunction;

struct WhatToExpect {
    Token::Type tokenType;
    Token::Literal literal;

    WhatToExpect(Token::Type tokenType) : tokenType(tokenType) {}
    WhatToExpect(Token::Type tokenType, double value)
        : tokenType(tokenType), literal(value)
    {
    }
    WhatToExpect(Token::Type tokenType, std::string value)
        : tokenType(tokenType), literal(value)
    {
    }
};

class ScannerTest : public ::testing::Test
{
  public:
    gravlax::Scanner scanner;
    std::unique_ptr<std::vector<Token>> tokens;

    void expectTokens(std::initializer_list<WhatToExpect> expectedTypes)
    {
        auto tokens_it = tokens->begin();
        auto expected_it = expectedTypes.begin();
        do {
            EXPECT_EQ(tokens_it->type, expected_it->tokenType);
            if (tokens_it->type == Token::Type::IDENTIFIER &&
                std::holds_alternative<std::string>(expected_it->literal)) {
                EXPECT_EQ(tokens_it->lexeme,
                          std::get<std::string>(expected_it->literal));
            }
            tokens_it++;
            expected_it++;
        } while (tokens_it != tokens->end() &&
                 expected_it != expectedTypes.end());
        EXPECT_EQ(tokens->size(), expectedTypes.size());
    }

    void dump(const std::vector<gravlax::Token> &tokens)
    {
        for (auto token : tokens) {
            fmt::print("{}\n", token);
        }
    }

    void expect(std::string code,
                std::initializer_list<WhatToExpect> expectedTypes)
    {
        tokens = scanner.scanString(code);
        // dump(tokens);
        expectTokens(expectedTypes);
    }
};

TEST_F(ScannerTest, DefaultConstructor) {}

TEST_F(ScannerTest, EmptyFile)
{
    expect("", {Token::Type::END_OF_FILE});
}

TEST_F(ScannerTest, OnlySemicolon)
{
    expect(";", {Token::Type::SEMICOLON, Token::Type::END_OF_FILE});
}

TEST_F(ScannerTest, NilVariable)
{
    expect("var foo;", {Token::Type::VAR,
                        {Token::Type::IDENTIFIER, "foo"},
                        Token::Type::SEMICOLON,
                        Token::Type::END_OF_FILE});
}

TEST_F(ScannerTest, NumericVariable_1)
{
    expect("var foo = 1;", {Token::Type::VAR,
                            {Token::Type::IDENTIFIER, "foo"},
                            Token::Type::EQUAL,
                            {Token::Type::NUMBER, 1},
                            Token::Type::SEMICOLON,
                            Token::Type::END_OF_FILE});
}

TEST_F(ScannerTest, NumericVariable_1_0)
{
    expect("var foo = 1.0;", {Token::Type::VAR,
                              {Token::Type::IDENTIFIER, "foo"},
                              Token::Type::EQUAL,
                              {Token::Type::NUMBER, 1.0},
                              Token::Type::SEMICOLON,
                              Token::Type::END_OF_FILE});
}

TEST_F(ScannerTest, NumericVariable_10)
{
    expect("var foo = 10;", {Token::Type::VAR,
                             {Token::Type::IDENTIFIER, "foo"},
                             Token::Type::EQUAL,
                             {Token::Type::NUMBER, 10},
                             Token::Type::SEMICOLON,
                             Token::Type::END_OF_FILE});
}

TEST_F(ScannerTest, PrintHelloWorld)
{
    expect("print \"Hello, world!\";", {Token::Type::PRINT,
                                        {Token::Type::STRING, "Hello, world!"},
                                        Token::Type::SEMICOLON,
                                        Token::Type::END_OF_FILE});
}

TEST_F(ScannerTest, Statement_1)
{
    expect("var average = (min + max) / 2;",
           {Token::Type::VAR,
            {Token::Type::IDENTIFIER, "average"},
            Token::Type::EQUAL,
            Token::Type::LEFT_PAREN,
            {Token::Type::IDENTIFIER, "min"},
            Token::Type::PLUS,
            {Token::Type::IDENTIFIER, "max"},
            Token::Type::RIGHT_PAREN,
            Token::Type::SLASH,
            {Token::Type::NUMBER, 10},
            Token::Type::SEMICOLON,
            Token::Type::END_OF_FILE});
}

TEST_F(ScannerTest, IfStatement)
{
    expect("if (condition) { print \"yes\"; } else { print \"no\"; }",
           {Token::Type::IF,
            Token::Type::LEFT_PAREN,
            {Token::Type::IDENTIFIER, "condition"},
            Token::Type::RIGHT_PAREN,
            Token::Type::LEFT_BRACE,
            Token::Type::PRINT,
            {Token::Type::STRING, "yes"},
            Token::Type::SEMICOLON,
            Token::Type::RIGHT_BRACE,
            Token::Type::ELSE,
            Token::Type::LEFT_BRACE,
            Token::Type::PRINT,
            {Token::Type::STRING, "no"},
            Token::Type::SEMICOLON,
            Token::Type::RIGHT_BRACE,
            Token::Type::END_OF_FILE});
}

TEST_F(ScannerTest, WhileStatement)
{
    expect("var a = 1; while (a < 10) {   print a;   a = a + 1; } ",
           {Token::Type::VAR,         {Token::Type::IDENTIFIER, "a"},
            Token::Type::EQUAL,       {Token::Type::NUMBER, 1},
            Token::Type::SEMICOLON,   Token::Type::WHILE,
            Token::Type::LEFT_PAREN,  {Token::Type::IDENTIFIER, "a"},
            Token::Type::LESS,        {Token::Type::NUMBER, 10},
            Token::Type::RIGHT_PAREN, Token::Type::LEFT_BRACE,
            Token::Type::PRINT,       {Token::Type::IDENTIFIER, "a"},
            Token::Type::SEMICOLON,   {Token::Type::IDENTIFIER, "a"},
            Token::Type::EQUAL,       {Token::Type::IDENTIFIER, "a"},
            Token::Type::PLUS,        {Token::Type::NUMBER, 1},
            Token::Type::SEMICOLON,   Token::Type::RIGHT_BRACE,
            Token::Type::END_OF_FILE});
}

TEST_F(ScannerTest, Symbols)
{
    expect(", . - * ! != > >= <=",
           {Token::Type::COMMA, Token::Type::DOT, Token::Type::MINUS,
            Token::Type::STAR, Token::Type::BANG, Token::Type::BANG_EQUAL,
            Token::Type::GREATER, Token::Type::GREATER_EQUAL,
            Token::Type::LESS_EQUAL, Token::Type::END_OF_FILE});
}

TEST_F(ScannerTest, Comment)
{
    expect("// comment", {Token::Type::END_OF_FILE});
}
