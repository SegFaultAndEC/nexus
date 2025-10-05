#ifndef NEXUS_TOKEN_HPP
#define NEXUS_TOKEN_HPP

#include <string>
#include <string_view>

namespace nx {
class Token {
  public:
    enum class Type {
        UNKNOWN = 0,
        IDENT,
        INTEGER,
        DECIMAL,
        STRING,
        TRUE_V,  // true
        FALSE_V, // false
        NIL,     // NIL

        PLUS,     // +
        MINUS,    // -
        ASTERISK, // *
        SLASH,    // /
        POWER,    // ^

        EQUAL,     // ==
        NOT_EQUAL, // !=
        GREATER,   // >
        GREATER_E, // >=
        LESS,      // <
        LESS_E,    // <=

        NOT, // not
        AND, // and
        OR,  // or

        ASSIGN, // =
        DOT,    // .
        PIPE,   // |

        LPAREN,   // (
        RPAREN,   // )
        LBRACKET, // [
        RBRACKET, // ]
        COMMA,    // ,

        COND_MARK, // ?

        END,
    };

    Token();

    Token(Type type, std::string_view literal, std::string_view nameSpace = "");

    Token(const Token &token);

    Token &operator=(const Token &token);

    std::string_view getLiteral() const;
    std::string_view getNameSpace() const;
    bool nsEmpty() const;

    Type getType() const;
    bool match(Type type) const;
    bool isEnd() const;
    bool isNotEnd() const;

    static std::string getName(Type type);

  private:
    Type _type{Type::UNKNOWN};
    std::string_view _literal{};
    std::string_view _nameSpace{};
};
}

#endif // NEXUS_TOKEN_HPP
