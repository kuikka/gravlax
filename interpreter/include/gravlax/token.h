#pragma once

#include <string>
#include <variant>

#include <fmt/format.h>

namespace gravlax
{

struct Token {
    enum Type {
        // Single-character tokens.
        LEFT_PAREN,
        RIGHT_PAREN,
        LEFT_BRACE,
        RIGHT_BRACE,
        COMMA,
        DOT,
        MINUS,
        PLUS,
        SEMICOLON,
        SLASH,
        STAR,

        // One or two character tokens.
        BANG,
        BANG_EQUAL,
        EQUAL,
        EQUAL_EQUAL,
        GREATER,
        GREATER_EQUAL,
        LESS,
        LESS_EQUAL,

        // Literals.
        IDENTIFIER,
        STRING,
        NUMBER,

        // Keywords.
        AND,
        CLASS,
        ELSE,
        FALSE,
        FUN,
        FOR,
        IF,
        NIL,
        OR,
        PRINT,
        RETURN,
        SUPER,
        THIS,
        TRUE,
        VAR,
        WHILE,

        END_OF_FILE
    };

    using Literal = std::variant<std::string, double>;

    Type type;
    std::string lexeme;
    Literal literal;
    int line;

    Token(Token::Type type, std::string lexeme, Literal literal, int line)
        : type(type), lexeme(lexeme), literal(literal), line(line)
    {
    }
};

}; // namespace gravlax

template <> struct fmt::formatter<::gravlax::Token> : formatter<string_view> {
    auto format(::gravlax::Token t,
                format_context &ctx) const -> format_context::iterator;
};