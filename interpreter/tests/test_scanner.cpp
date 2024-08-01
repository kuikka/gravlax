#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <lox/scanner.h>

using ::lox::Token;
using ::testing::_;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::MockFunction;

class ScannerTest : public ::testing::Test {
public:
  lox::Scanner scanner;
  void expectTokens(std::vector<Token> tokens,
                    std::initializer_list<Token::Type> expectedTypes) {
    auto tokens_it = tokens.begin();
    auto expected_it = expectedTypes.begin();
    do {
      EXPECT_EQ(tokens_it->type, *expected_it);
      tokens_it++;
      expected_it++;
    } while (tokens_it != tokens.end() && expected_it != expectedTypes.end());
    EXPECT_EQ(tokens.size(), expectedTypes.size());
  }
};

TEST_F(ScannerTest, DefaultConstructor) {}

TEST_F(ScannerTest, EmptyFile) {
  auto tokens = scanner.scanString("");
  EXPECT_EQ(tokens.size(), 1); // End token
  expectTokens(tokens, {Token::Type::END_OF_FILE});
}

TEST_F(ScannerTest, OnlySemicolon) {
  auto tokens = scanner.scanString(";");
  EXPECT_EQ(tokens.size(), 2);
  expectTokens(tokens, {Token::Type::SEMICOLON, Token::Type::END_OF_FILE});
}

TEST_F(ScannerTest, NilVariable) {
  auto tokens = scanner.scanString("var foo;");
  EXPECT_EQ(tokens.size(), 4);
  expectTokens(tokens, {Token::Type::VAR, Token::Type::IDENTIFIER,
                        Token::Type::SEMICOLON, Token::Type::END_OF_FILE});
}

TEST_F(ScannerTest, NumericVariable_1) {
  auto tokens = scanner.scanString("var foo = 1;");
  EXPECT_EQ(tokens.size(), 6);
  expectTokens(tokens, {Token::Type::VAR, Token::Type::IDENTIFIER,
                        Token::Type::EQUAL, Token::Type::NUMBER,
                        Token::Type::SEMICOLON, Token::Type::END_OF_FILE});
  EXPECT_EQ(tokens[1].lexeme, "foo");
  EXPECT_EQ(std::get<double>(tokens[3].literal), 1.0f);
}

TEST_F(ScannerTest, NumericVariable_1_0) {
  auto tokens = scanner.scanString("var foo = 1.0;");
  EXPECT_EQ(tokens.size(), 6);
  expectTokens(tokens, {Token::Type::VAR, Token::Type::IDENTIFIER,
                        Token::Type::EQUAL, Token::Type::NUMBER,
                        Token::Type::SEMICOLON, Token::Type::END_OF_FILE});
  EXPECT_EQ(tokens[1].lexeme, "foo");
  EXPECT_EQ(std::get<double>(tokens[3].literal), 1.0f);
}
