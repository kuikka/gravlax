#pragma once

#include <map>
#include <memory>
#include <vector>

#include <gravlax/token.h>

namespace gravlax
{

class Scanner
{
    std::vector<Token> tokens;
    std::string code;
    std::map<std::string, Token::Type> keywords;

    int start = 0;
    int current = 0;
    int line = 1;

    bool hadError = false;

    void scanTokens();
    void scanToken();

    bool isAtEnd();
    char advance();
    bool match(char expected);
    char peek();
    char peekNext();

    void identifier();
    bool isAlpha(char c);
    bool isAlphaNumeric(char c);

    void string();
    char isDigit(char c);
    void number();

    void error(int line, std::string message);

    void addToken(Token::Type tokenType);
    void addToken(Token::Type tokenType, Token::Literal literal);

  public:
    Scanner();
    ~Scanner();

    std::unique_ptr<std::vector<Token>> scanString(const std::string &code);
};
}; // namespace gravlax