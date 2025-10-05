#ifndef NEXUS_NODE_HPP
#define NEXUS_NODE_HPP

#include "lexer/token.hpp"
#include <vector>

namespace nx {
struct ASTNode {
    enum class Type {
        UNKNOWN = 0,
        CONSTANT, // 常量
        LIST,     // 列表常量
        IDENT,    // 标识符，一般为变量名，也可能为函数名
        FUNCTION, // 函数
        INDEX,    // 下标运算符
        PREFIX,   // 前缀运算符，现仅用于负号
        BINARY,   // 二元运算符
    };
    ASTNode();
    explicit ASTNode(const Token &token);
    ASTNode(const Token &token, Type type);
    ASTNode(const Token &token, Type type, ASTNode *node);
    ASTNode(const Token &token, Type type, ASTNode *left, ASTNode *right);
    ~ASTNode();
    Type getType() const;
    const Token &getToken() const;
    const std::vector<ASTNode *> &getNodes() const;
    ASTNode *addNode(ASTNode *node);
    bool match(Type type) const;

  private:
    Token _token{};
    Type _type{Type::UNKNOWN};
    std::vector<ASTNode *> _nodes;
};
} // namespace nx

#endif // NEXUS_NODE_HPP
