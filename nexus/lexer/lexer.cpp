#include "lexer.hpp"
namespace nx {
std::vector<Token> lexer(std::string_view line) {
    std::vector<Token> tokens;
    size_t i = 0;

    auto addToken = [&](Token::Type type, std::string_view lit,
                        std::string_view nameSpace = "") {
        tokens.emplace_back(type, lit, nameSpace);
    };

    while (i < line.size()) {
        char c = line[i];

        // 跳过空白
        if (std::isspace(static_cast<unsigned char>(c))) {
            ++i;
            continue;
        }

        // 注释直接跳出
        if (c == ';')
            break;

        // 数字：整数或小数
        if (std::isdigit(static_cast<unsigned char>(c))) {
            size_t start = i;
            while (i < line.size() &&
                   std::isdigit(static_cast<unsigned char>(line[i])))
                ++i;

            bool isDecimal = false;
            if (i < line.size() && line[i] == '.') {
                isDecimal = true;
                ++i; // 吃掉小数点
                while (i < line.size() &&
                       std::isdigit(static_cast<unsigned char>(line[i])))
                    ++i;
            }

            addToken(isDecimal ? Token::Type::DECIMAL : Token::Type::INTEGER,
                     line.substr(start, i - start));
            continue;
        }

        // 字符串（单引号或双引号）
        if (c == '"' || c == '\'') {
            char quote = c;
            size_t start = i++;
            while (i < line.size() && line[i] != quote) {
                if (line[i] == '\\' && i + 1 < line.size())
                    i += 2;
                else
                    ++i;
            }
            if (i < line.size())
                ++i; // 吃掉结尾引号
            addToken(Token::Type::STRING, line.substr(start, i - start));
            continue;
        }

        // 标识符或关键字
        if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
            size_t start = i;
            while (i < line.size() &&
                   (std::isalnum(static_cast<unsigned char>(line[i])) ||
                    line[i] == '_'))
                ++i;
            if (i + 2 < line.size() && line[i] == '.' && line[i + 1] == '.' &&
                (std::isalpha(static_cast<unsigned char>(line[i + 2])) ||
                 line[i + 2] == '_')) {

                std::string_view ns = line.substr(start, i - start);
                i += 2; // 跳过 "->"

                start = i;
                while (i < line.size() &&
                       (std::isalnum(static_cast<unsigned char>(line[i])) ||
                        line[i] == '_'))
                    ++i;

                std::string_view lit = line.substr(start, i - start);
                addToken(Token::Type::IDENT, lit, ns);
                continue;
            }
            std::string_view lit = line.substr(start, i - start);
            if (lit == "and")
                addToken(Token::Type::AND, lit);
            else if (lit == "or")
                addToken(Token::Type::OR, lit);
            else if (lit == "not")
                addToken(Token::Type::NOT, lit);
            else if (lit == "true")
                addToken(Token::Type::TRUE_V, lit);
            else if (lit == "false")
                addToken(Token::Type::FALSE_V, lit);
            else if (lit == "nil")
                addToken(Token::Type::NIL, lit);
            else
                addToken(Token::Type::IDENT, lit);
            continue;
        }

        // 双字符运算符
        if (i + 1 < line.size()) {
            std::string_view two = line.substr(i, 2);
            if (two == "==") {
                addToken(Token::Type::EQUAL, two);
                i += 2;
                continue;
            }
            if (two == "!=") {
                addToken(Token::Type::NOT_EQUAL, two);
                i += 2;
                continue;
            }
            if (two == ">=") {
                addToken(Token::Type::GREATER_E, two);
                i += 2;
                continue;
            }
            if (two == "<=") {
                addToken(Token::Type::LESS_E, two);
                i += 2;
                continue;
            }
        }

        // 单字符运算符/符号
        switch (c) {
        case '+':
            addToken(Token::Type::PLUS, line.substr(i, 1));
            break;
        case '-':
            addToken(Token::Type::MINUS, line.substr(i, 1));
            break;
        case '*':
            addToken(Token::Type::ASTERISK, line.substr(i, 1));
            break;
        case '/':
            addToken(Token::Type::SLASH, line.substr(i, 1));
            break;
        case '^':
            addToken(Token::Type::POWER, line.substr(i, 1));
            break;
        case '>':
            addToken(Token::Type::GREATER, line.substr(i, 1));
            break;
        case '<':
            addToken(Token::Type::LESS, line.substr(i, 1));
            break;
        case '(':
            addToken(Token::Type::LPAREN, line.substr(i, 1));
            break;
        case ')':
            addToken(Token::Type::RPAREN, line.substr(i, 1));
            break;
        case '[':
            addToken(Token::Type::LBRACKET, line.substr(i, 1));
            break;
        case ']':
            addToken(Token::Type::RBRACKET, line.substr(i, 1));
            break;
        case ',':
            addToken(Token::Type::COMMA, line.substr(i, 1));
            break;
        case '.':
            addToken(Token::Type::DOT, line.substr(i, 1));
            break;
        case '|':
            addToken(Token::Type::PIPE, line.substr(i, 1));
            break;
        case '=':
            addToken(Token::Type::ASSIGN, line.substr(i, 1));
            break;
        case '?':
            addToken(Token::Type::END, "");
            addToken(Token::Type::COND_MARK, line.substr(i, 1));
            break;
        default:
            addToken(Token::Type::UNKNOWN, line.substr(i, 1));
            break;
        }
        ++i;
    }

    addToken(Token::Type::END, "");
    return tokens;
}
} // namespace nx