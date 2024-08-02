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
    std::vector<Token> tokens;

    void expectTokens(std::initializer_list<WhatToExpect> expectedTypes)
    {
        auto tokens_it = tokens.begin();
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
        } while (tokens_it != tokens.end() &&
                 expected_it != expectedTypes.end());
        EXPECT_EQ(tokens.size(), expectedTypes.size());
    }

    void expect(std::string code,
                std::initializer_list<WhatToExpect> expectedTypes)
    {
        tokens = scanner.scanString(code);
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
    EXPECT_EQ(tokens[1].lexeme, "foo");
    EXPECT_EQ(std::get<double>(tokens[3].literal), 1.0f);
}

TEST_F(ScannerTest, NumericVariable_1_0)
{
    expect("var foo = 1.0;", {Token::Type::VAR,
                              {Token::Type::IDENTIFIER, "foo"},
                              Token::Type::EQUAL,
                              {Token::Type::NUMBER, 1.0},
                              Token::Type::SEMICOLON,
                              Token::Type::END_OF_FILE});
    EXPECT_EQ(tokens[1].lexeme, "foo");
    EXPECT_EQ(std::get<double>(tokens[3].literal), 1.0f);
}
