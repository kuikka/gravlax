#include <gravlax/token.h>

namespace
{
using namespace gravlax;

std::string type_to_string(Token::Type type)
{
    switch (type) {
    case Token::Type::LEFT_PAREN:
        return "LEFT_PAREN";
    case Token::Type::RIGHT_PAREN:
        return "RIGHT_PAREN";
    case Token::Type::LEFT_BRACE:
        return "LEFT_BRACE";
    case Token::Type::RIGHT_BRACE:
        return "RIGHT_BRACE";
    case Token::Type::COMMA:
        return "COMMA";
    case Token::Type::DOT:
        return "DOT";
    case Token::Type::MINUS:
        return "MINUS";
    case Token::Type::PLUS:
        return "PLUS";
    case Token::Type::SEMICOLON:
        return "SEMICOLON";
    case Token::Type::SLASH:
        return "SLASH";
    case Token::Type::STAR:
        return "STAR";

    case Token::Type::BANG:
        return "BANG";
    case Token::Type::BANG_EQUAL:
        return "BANG_EQUAL";
    case Token::Type::EQUAL:
        return "EQUAL";
    case Token::Type::EQUAL_EQUAL:
        return "EQUAL_EQUAL";
    case Token::Type::GREATER:
        return "GREATER";
    case Token::Type::GREATER_EQUAL:
        return "GREATER_EQUAL";
    case Token::Type::LESS:
        return "LESS";
    case Token::Type::LESS_EQUAL:
        return "LESS_EQUAL";

    case Token::Type::IDENTIFIER:
        return "IDENTIFIER";
    case Token::Type::STRING:
        return "STRING";
    case Token::Type::NUMBER:
        return "NUMBER";

    case Token::Type::AND:
        return "AND";
    case Token::Type::CLASS:
        return "CLASS";
    case Token::Type::ELSE:
        return "ELSE";
    case Token::Type::FALSE:
        return "FALSE";
    case Token::Type::FUN:
        return "FUN";
    case Token::Type::FOR:
        return "FOR";
    case Token::Type::IF:
        return "IF";
    case Token::Type::NIL:
        return "NIL";
    case Token::Type::OR:
        return "OR";
    case Token::Type::PRINT:
        return "PRINT";
    case Token::Type::RETURN:
        return "RETURN";
    case Token::Type::SUPER:
        return "SUPER";
    case Token::Type::THIS:
        return "THIS";
    case Token::Type::TRUE:
        return "TRUE";
    case Token::Type::VAR:
        return "VAR";
    case Token::Type::WHILE:
        return "WHILE";

    case Token::Type::END_OF_FILE:
        return "END_OF_FILE";

    default:
        return "Unknown!";
    }
}

std::string literal_to_string(Token::Literal literal)
{
    if (std::holds_alternative<double>(literal)) {
        return std::to_string(std::get<double>(literal));
    }
    if (std::holds_alternative<std::string>(literal)) {
        return std::get<std::string>(literal);
    }
    return std::string("n/a");
}

}; // namespace

auto fmt::formatter<::gravlax::Token>::format(::gravlax::Token token,
                                              format_context &ctx) const
    -> format_context::iterator
{
    std::string desc =
        fmt::format("Token({}, {}, \"{}\", {})", type_to_string(token.type),
                    token.lexeme, literal_to_string(token.literal), token.line);
    return formatter<string_view>::format(desc, ctx);
}
