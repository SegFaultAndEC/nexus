#include "node.hpp"

namespace nx {
ASTNode::ASTNode() = default;
ASTNode::ASTNode(const Token &token) : _token(token) {}
ASTNode::ASTNode(const Token &token, ASTNode::Type type)
    : _token(token), _type(type) {}
ASTNode::ASTNode(const Token &token, ASTNode::Type type, ASTNode *node)
    : _token(token), _type(type) {
    _nodes.resize(1);
    _nodes[0] = node;
}
ASTNode::ASTNode(const Token &token, ASTNode::Type type, ASTNode *left,
                 ASTNode *right)
    : _token(token), _type(type) {
    _nodes.resize(2);
    _nodes[0] = left;
    _nodes[1] = right;
}

ASTNode::~ASTNode() = default;
ASTNode::Type ASTNode::getType() const { return _type; }
const Token &ASTNode::getToken() const { return _token; }
bool ASTNode::match(ASTNode::Type type) const { return _type == type; }
ASTNode *ASTNode::addNode(ASTNode *node) {
    _nodes.emplace_back(node);
    return node;
}
const std::vector<ASTNode *> &ASTNode::getNodes() const { return _nodes; }
} // namespace nx
