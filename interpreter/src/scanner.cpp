#include <fmt/core.h>
#include <gravlax/scanner.h>
#include <iostream>

namespace gravlax
{

Scanner::Scanner()
{
    keywords["and"] = Token::Type::AND;
    keywords["class"] = Token::Type::CLASS;
    keywords["else"] = Token::Type::ELSE;
    keywords["false"] = Token::Type::FALSE;
    keywords["for"] = Token::Type::FOR;
    keywords["fun"] = Token::Type::FUN;
    keywords["if"] = Token::Type::IF;
    keywords["nil"] = Token::Type::NIL;
    keywords["or"] = Token::Type::OR;
    keywords["print"] = Token::Type::PRINT;
    keywords["return"] = Token::Type::RETURN;
    keywords["super"] = Token::Type::SUPER;
    keywords["this"] = Token::Type::THIS;
    keywords["true"] = Token::Type::TRUE;
    keywords["var"] = Token::Type::VAR;
    keywords["while"] = Token::Type::WHILE;
}

Scanner::~Scanner() {}

std::unique_ptr<std::vector<Token>> Scanner::scanString(const std::string &code)
{
    this->code = code;
    scanTokens();
    return std::make_unique<std::vector<Token>>(std::move(tokens));
}

void Scanner::scanTokens()
{
    while (!isAtEnd()) {
        // We are at the beginning of the next lexeme.
        start = current;
        scanToken();
    }

    tokens.push_back(Token(Token::Type::END_OF_FILE, "", {}, line));
}

bool Scanner::match(char expected)
{
    if (isAtEnd())
        return false;
    if (code[current] != expected)
        return false;

    current++;
    return true;
}

char Scanner::peek()
{
    if (isAtEnd()) {
        return '\0';
    }
    return code[current];
}

char Scanner::isDigit(char c)
{
    if (c >= '0' && c <= '9') {
        return true;
    }
    return false;
}

char Scanner::peekNext()
{
    if (current + 1 >= code.length())
        return '\0';
    return code[current + 1];
}

void Scanner::number()
{
    while (isDigit(peek()))
        advance();

    // Look for a fractional part.
    if (peek() == '.' && isDigit(peekNext())) {
        // Consume the "."
        advance();

        while (isDigit(peek()))
            advance();
    }

    double value = std::stod(code.substr(start, current - start));
    addToken(Token::Type::NUMBER, value);
}

void Scanner::scanToken()
{
    char c = advance();
    // fmt::print("Found character {} at offset {}\n", c, current - 1);
    switch (c) {
    case '(':
        addToken(Token::Type::LEFT_PAREN);
        break;
    case ')':
        addToken(Token::Type::RIGHT_PAREN);
        break;
    case '{':
        addToken(Token::Type::LEFT_BRACE);
        break;
    case '}':
        addToken(Token::Type::RIGHT_BRACE);
        break;
    case ',':
        addToken(Token::Type::COMMA);
        break;
    case '.':
        addToken(Token::Type::DOT);
        break;
    case '-':
        addToken(Token::Type::MINUS);
        break;
    case '+':
        addToken(Token::Type::PLUS);
        break;
    case ';':
        addToken(Token::Type::SEMICOLON);
        break;
    case '*':
        addToken(Token::Type::STAR);
        break;
    case '!':
        addToken(match('=') ? Token::Type::BANG_EQUAL : Token::Type::BANG);
        break;
    case '=':
        addToken(match('=') ? Token::Type::EQUAL_EQUAL : Token::Type::EQUAL);
        break;
    case '<':
        addToken(match('=') ? Token::Type::LESS_EQUAL : Token::Type::LESS);
        break;
    case '>':
        addToken(match('=') ? Token::Type::GREATER_EQUAL
                            : Token::Type::GREATER);
        break;
    case '/':
        if (match('/')) {
            // A comment goes until the end of the line.
            while (peek() != '\n' && !isAtEnd()) {
                advance();
            }
        } else {
            addToken(Token::Type::SLASH);
        }
        break;
    case ' ':
    case '\r':
    case '\t':
        // Ignore whitespace.
        break;

    case '\n':
        line++;
        break;

    case '"':
        string();
        break;

    default:
        if (isDigit(c)) {
            number();
        } else if (isAlpha(c)) {
            identifier();
        } else {
            error(line, fmt::format("Unexpected character \'{}\'", c));
        }
        break;
    }
}

void Scanner::string()
{
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n')
            line++;
        advance();
    }

    if (isAtEnd()) {
        error(line, "Unterminated string.");
        return;
    }

    // The closing ".
    advance();

    // Trim the surrounding quotes.
    std::string value = code.substr(start + 1, current - start - 2);
    addToken(Token::Type::STRING, value);
}

void Scanner::identifier()
{
    while (isAlphaNumeric(peek()))
        advance();

    std::string text = code.substr(start, current - start);
    auto it = keywords.find(text);

    if (it == keywords.end()) {
        addToken(Token::Type::IDENTIFIER);
    } else {
        addToken(it->second);
    }
}

bool Scanner::isAlpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Scanner::isAlphaNumeric(char c)
{
    return isAlpha(c) || isDigit(c);
}

void Scanner::error(int line, std::string msg)
{
    hadError = true;
    std::cout << msg << std::endl;
}

bool Scanner::isAtEnd()
{
    return current >= code.length();
}

char Scanner::advance()
{
    return code[current++];
}

void Scanner::addToken(Token::Type tokenType)
{
    addToken(tokenType, {});
}

void Scanner::addToken(Token::Type tokenType, Token::Literal literal)
{
    std::string lexeme = code.substr(start, current - start);
    tokens.push_back(Token(tokenType, lexeme, literal, line));
}

}; // namespace gravlax