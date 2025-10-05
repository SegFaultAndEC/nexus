#ifndef NEXUS_PARSER_HPP
#define NEXUS_PARSER_HPP

#include "node.hpp"
#include <span>

namespace nx {
template <class T> struct PoolCollectGuarder {
    PoolCollectGuarder(T f) : _f(f) {}
    ~PoolCollectGuarder() { _f(); }

  private:
    T _f;
};

void collectPool(std::vector<ASTNode *> &pool);

struct BindingPower {
    int left{};
    int right{};
};

class Parser {
    using PoolT = std::vector<ASTNode *>;

  public:
    Parser(std::span<Token> tokens, PoolT &pool);

    ASTNode *parseExpression(int rbp = 0);

  private:
    std::span<Token> _tokens;
    size_t _pos{0};
    PoolT &_pool;

    const Token &peek(size_t offset = 0) const;
    Token advance();

    ASTNode *nud(const Token &t);
    ASTNode *led(const Token &t, ASTNode *left);
};
} // namespace nx

#endif // NEXUS_PARSER_HPP
