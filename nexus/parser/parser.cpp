#include "parser.hpp"
#include "error/error.hpp"

namespace nx {
static BindingPower getBindingPower(const Token &t) {
    using TT = Token::Type;
    switch (t.getType()) {
    case TT::LBRACKET:
        return {100, 101};
    case TT::DOT:
        return {90, 91};
    case TT::POWER:
        return {81, 80};
    case TT::ASTERISK:
    case TT::SLASH:
        return {70, 71};
    case TT::PLUS:
    case TT::MINUS:
        return {60, 61};
    case TT::EQUAL:
    case TT::NOT_EQUAL:
    case TT::GREATER:
    case TT::GREATER_E:
    case TT::LESS:
    case TT::LESS_E:
        return {50, 51};
    case TT::AND:
        return {45, 46};
    case TT::OR:
        return {40, 41};
    case TT::ASSIGN:
        return {31, 30};
    case TT::PIPE:
        return {10, 11};
    default:
        return {0, 0};
    }
}
static int rbpPrefix() { return 100; }

Parser::Parser(std::span<Token> tokens, PoolT &pool)
    : _tokens(tokens), _pool(pool) {}

ASTNode *Parser::parseExpression(int rbp) {
    if (_tokens.begin()->isEnd())
        return nullptr;
    Token t = advance();
    ASTNode *left = nud(t);
    while (rbp < getBindingPower(peek()).left) {
        t = advance();
        left = led(t, left);
    }
    return left;
}

const Token &Parser::peek(size_t offset) const {
    if (_pos + offset >= _tokens.size()) {
        return _tokens.back(); // 最后一个固定是 END
    }
    return _tokens[_pos + offset];
}

Token Parser::advance() {
    if (_pos < _tokens.size()) {
        return _tokens[_pos++];
    }
    return _tokens.back(); // 返回 END
}

ASTNode *Parser::nud(const Token &t) {
    using TT = Token::Type;
    using NT = ASTNode::Type;
    switch (t.getType()) {
    case TT::INTEGER:
    case TT::DECIMAL:
    case TT::STRING:
    case TT::TRUE_V:
    case TT::FALSE_V:
    case TT::NIL:
        return _pool.emplace_back(new ASTNode(t, NT::CONSTANT));
    case TT::LBRACKET: {
        auto list = _pool.emplace_back(new ASTNode(t, NT::LIST));
        // 参数列表
        if (!peek().match(TT::RBRACKET)) {
            while (true) {
                auto arg = parseExpression();
                list->addNode(arg);
                if (peek().match(TT::COMMA)) {
                    advance(); // 消耗 ','
                    auto a = peek();
                    if (peek().match(TT::RBRACKET))
                        break;
                    continue;
                }
                break;
            }
        }
        if (!peek().match(TT::RBRACKET))
            NX_ERROR("Error0x0012:Unexpected ']'.")
        advance(); // 消耗 ']'
        return list;
    }
    case TT::IDENT: {
        // 识别为函数传参
        if (peek().match(TT::LPAREN)) {
            advance();
            auto func = _pool.emplace_back(new ASTNode(t, NT::FUNCTION));
            // 参数列表
            if (!peek().match(TT::RPAREN)) {
                while (true) {
                    auto arg = parseExpression();
                    func->addNode(arg);
                    if (peek().match(TT::COMMA)) {
                        advance(); // 消耗 ','
                        continue;
                    }
                    break;
                }
            }

            if (!peek().match(TT::RPAREN))
                NX_ERROR("Error0x0002:Unexpected ')'.")
            advance(); // 消耗 ')'
            return func;
        }
        return _pool.emplace_back(new ASTNode(t, NT::IDENT));
    }
    // 前缀
    case TT::MINUS:
    case TT::NOT: {
        ASTNode *node = parseExpression(rbpPrefix());
        return _pool.emplace_back(new ASTNode(t, ASTNode::Type::PREFIX, node));
    }
    case TT::LPAREN: {
        ASTNode *expr = parseExpression();
        if (!peek().match(TT::RPAREN))
            NX_ERROR("Error0x0002:Unexpected ')'.")
        advance(); // 消耗 ')'
        return expr;
    }
    default:
        NX_ERROR("Error0x0009:Unexpected token '{}' in nud.", t.getLiteral());
    }
}

ASTNode *Parser::led(const Token &t, ASTNode *left) {
    using NT = ASTNode::Type;
    using TT = Token::Type;
    switch (t.getType()) {
    case TT::PLUS:
    case TT::MINUS:
    case TT::ASTERISK:
    case TT::SLASH:
    case TT::POWER:
    case TT::EQUAL:
    case TT::NOT_EQUAL:
    case TT::GREATER:
    case TT::GREATER_E:
    case TT::LESS:
    case TT::LESS_E:
    case TT::NOT:
    case TT::AND:
    case TT::OR:
    case TT::ASSIGN:
    case TT::DOT:
    case TT::PIPE: {
        BindingPower bp = getBindingPower(t);
        ASTNode *right = parseExpression(bp.right);
        return _pool.emplace_back(new ASTNode(t, NT::BINARY, left, right));
    }
    case TT::LBRACKET: {
        auto index = _pool.emplace_back(new ASTNode(t, NT::INDEX));
        index->addNode(left);
        // 参数列表
        if (!peek().match(TT::RBRACKET)) {
            while (true) {
                auto arg = parseExpression();
                index->addNode(arg);
                if (peek().match(TT::COMMA)) {
                    advance(); // 消耗 ','
                    continue;
                }
                break;
            }
        }

        if (!peek().match(TT::RBRACKET))
            NX_ERROR("Error0x0012:Unexpected ']'.")
        advance(); // 消耗 ']'
        return index;
    }
    default:
        NX_ERROR("Error0x0008:Unexpected token '{}' in led.", t.getLiteral());
    }
}

void collectPool(std::vector<ASTNode *> &pool) {
    for (auto i : pool) {
        delete i;
    }
    pool.clear();
}
} // namespace nx
