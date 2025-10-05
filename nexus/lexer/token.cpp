#include "token.hpp"

namespace nx {
Token::Token() = default;

Token::Token(Token::Type type, std::string_view literal,
             std::string_view nameSpace)
    : _type(type), _literal(literal), _nameSpace(nameSpace) {}

Token::Token(const Token &token) {
    if (&token == this) {
        return;
    }
    _type = token._type;
    _literal = token._literal;
    _nameSpace = token._nameSpace;
}

Token &Token::operator=(const Token &token) {
    if (&token == this) {
        return *this;
    }
    _type = token._type;
    _literal = token._literal;
    _nameSpace = token._nameSpace;
    return *this;
}

std::string_view Token::getLiteral() const { return _literal; }
std::string_view Token::getNameSpace() const { return _nameSpace; }
bool Token::nsEmpty() const { return _nameSpace.empty(); }

Token::Type Token::getType() const { return _type; }
bool Token::match(Token::Type type) const { return _type == type; }
bool Token::isEnd() const { return _type == Type::END; }
bool Token::isNotEnd() const { return !isEnd(); }

std::string Token::getName(Token::Type type) {
    switch (type) {
    case Type::UNKNOWN:
        return "UNKNOWN";
    case Type::INTEGER:
        return "INTEGER";
    case Type::DECIMAL:
        return "DECIMAL";
    case Type::STRING:
        return "STRING";
    case Type::NIL:
        return "NIL";
    case Type::IDENT:
        return "IDENT";

    case Type::PLUS:
        return "PLUS";
    case Type::MINUS:
        return "MINUS";
    case Type::ASTERISK:
        return "ASTERISK";
    case Type::SLASH:
        return "SLASH";

    case Type::EQUAL:
        return "EQUAL";
    case Type::GREATER:
        return "GREATER";
    case Type::GREATER_E:
        return "GREATER_E";
    case Type::LESS:
        return "LESS";
    case Type::LESS_E:
        return "LESS_E";
    case Type::NOT:
        return "NOT";

    case Type::AND:
        return "AND";
    case Type::OR:
        return "OR";

    case Type::LPAREN:
        return "LPAREN";
    case Type::RPAREN:
        return "RPAREN";
    case Type::COMMA:
        return "COMMA";
    case Type::DOT:
        return "DOT";
    case Type::ASSIGN:
        return "ARROW";
    case Type::PIPE:
        return "PIPE";
    case Type::TRUE_V:
        return "TRUE";
    case Type::FALSE_V:
        return "FALSE";
    case Type::POWER:
        return "POWER";
    case Type::NOT_EQUAL:
        return "NOT_EQUAL";
    case Type::LBRACKET:
        return "LBRACKET";
    case Type::RBRACKET:
        return "RBRACKET";
    case Type::END:
        return "END";
    }
    return "UNKNOWN";
}
} // namespace nx