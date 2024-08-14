#pragma once

#include <memory>
#include <vector>

#include <gravlax/expression.h>
#include <gravlax/token.h>

#include <gravlax/generated/binary.h>
#include <gravlax/generated/grouping.h>
#include <gravlax/generated/literal.h>
#include <gravlax/generated/unary.h>

namespace gravlax
{
using gravlax::generated::Binary;
using gravlax::generated::Literal;
using gravlax::generated::Unary;

class ParseError : public std::runtime_error
{
  public:
    ParseError() : std::runtime_error("") {}
    ParseError(std::string message) : std::runtime_error(message) {}
};

template <typename R> class Parser
{
    std::unique_ptr<std::vector<Token>> tokens;
    int current = 0;

    std::shared_ptr<Expr<R>> expression() { return equality(); }

    std::shared_ptr<Expr<R>> equality()
    {
        std::shared_ptr<Expr<R>> expr = comparison();

        while (match(Token::Type::BANG_EQUAL, Token::Type::EQUAL_EQUAL)) {
            Token oper = previous();
            std::shared_ptr<Expr<R>> right = comparison();
            expr = std::make_shared<Binary<R>>(expr, oper, right);
        }

        return expr;
    }

    bool match(auto... types)
    {
        for (auto type : {types...}) {
            if (check(type)) {
                advance();
                return true;
            }
        }

        return false;
    }

    bool check(Token::Type type)
    {
        if (isAtEnd())
            return false;
        return peek().type == type;
    }

    Token advance()
    {
        if (!isAtEnd())
            current++;
        return previous();
    }

    bool isAtEnd() { return peek().type == Token::Type::END_OF_FILE; }

    Token peek() { return (*tokens)[current]; }

    Token previous() { return (*tokens)[current - 1]; }

    std::shared_ptr<Expr<R>> comparison()
    {
        std::shared_ptr<Expr<R>> expr = term();

        while (match(Token::Type::GREATER, Token::Type::GREATER_EQUAL,
                     Token::Type::LESS, Token::Type::LESS_EQUAL)) {
            Token oper = previous();
            std::shared_ptr<Expr<R>> right = term();
            expr = std::make_shared<Binary<R>>(expr, oper, right);
        }

        return expr;
    }

    std::shared_ptr<Expr<R>> term()
    {
        std::shared_ptr<Expr<R>> expr = factor();

        while (match(Token::Type::MINUS, Token::Type::PLUS)) {
            Token oper = previous();
            std::shared_ptr<Expr<R>> right = factor();
            expr = std::make_shared<Binary<R>>(expr, oper, right);
        }

        return expr;
    }

    std::shared_ptr<Expr<R>> factor()
    {
        std::shared_ptr<Expr<R>> expr = unary();

        while (match(Token::Type::SLASH, Token::Type::STAR)) {
            Token oper = previous();
            std::shared_ptr<Expr<R>> right = unary();
            expr = std::make_shared<Binary<R>>(expr, oper, right);
        }

        return expr;
    }

    std::shared_ptr<Expr<R>> unary()
    {
        if (match(Token::Type::BANG, Token::Type::MINUS)) {
            Token oper = previous();
            std::shared_ptr<Expr<R>> right = unary();
            return std::make_shared<Unary<R>>(oper, right);
        }

        return primary();
    }

    std::shared_ptr<Expr<R>> primary()
    {
        if (match(Token::Type::FALSE))
            return std::make_shared<Literal<R>>(false);
        if (match(Token::Type::TRUE))
            return std::make_shared<Literal<R>>(true);
        if (match(Token::Type::NIL))
            return std::make_shared<Literal<R>>(Token::Literal());

        if (match(Token::Type::NUMBER, Token::Type::STRING)) {
            return std::make_shared<Literal<R>>(previous().literal);
        }

        if (match(Token::Type::LEFT_PAREN)) {
            auto expr = expression();
            consume(Token::Type::RIGHT_PAREN, "Expect ')' after expression.");
            return expr; // std::make_shared<Grouping<R>>(expr);
        }

        throw ParseError("Unknown token!");
    }

    void synchronize()
    {
        advance();

        while (!isAtEnd()) {
            if (previous().type == Token::Type::SEMICOLON)
                return;

            switch (peek().type) {
            case Token::Type::CLASS:
            case Token::Type::FUN:
            case Token::Type::VAR:
            case Token::Type::FOR:
            case Token::Type::IF:
            case Token::Type::WHILE:
            case Token::Type::PRINT:
            case Token::Type::RETURN:
                return;
            }

            advance();
        }
    }

    Token consume(Token::Type type, std::string message)
    {
        if (check(type))
            return advance();

        throw error(peek(), message);
    }

    ParseError error(Token token, std::string message)
    {
        return ParseError(message);
    }

  public:
    std::shared_ptr<Expr<R>> parse(std::unique_ptr<std::vector<Token>> tokens)
    {
        this->tokens = std::move(tokens);

        try {
            return expression();
        } catch (ParseError error) {
            return {};
        };
    }
};

}; // namespace gravlax