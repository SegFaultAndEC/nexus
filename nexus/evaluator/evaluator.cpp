#include "evaluator.hpp"
#include "error/error.hpp"
#include "function/operator_func.hpp"
#include "types/list.hpp"
#include "types/string.hpp"
#include <charconv>
#include <stdexcept>

namespace nx {
Evaluator::Evaluator(State &state) : _state(state) {}
Object Evaluator::eval(const ASTNode *node) {
    if (!node)
        return {};
    using NT = ASTNode::Type;
    switch (node->getType()) {
    case NT::CONSTANT:
        return evalConstant(node->getToken());
    case NT::LIST:
        return evalList(node);
    case NT::IDENT:
        return evalVar(node->getToken());
    case NT::PREFIX:
        return evalPrefix(node);
    case NT::BINARY:
        return evalBinaryOp(node);
    case NT::FUNCTION:
        return evalFunction(node);
    case NT::INDEX:
        return evalSubscript(node);
    default:
        return {};
    }
}
Object Evaluator::evalAssign(const ASTNode *node) {
    using NT = ASTNode::Type;
    auto left = node->getNodes()[0];
    if (left->match(NT::IDENT)) {
        auto right = node->getNodes()[1];
        auto var = eval(right);
        _state.addVar(std::string(left->getToken().getLiteral()), var);
        return var;
    }

    auto leftv = eval(left);
    if (leftv.isRValue()) {
        NX_ERROR("Error0x0001:Expression is not assignable.")
    }
    auto right = node->getNodes()[1];
    auto var = eval(right);
    ArgsFixedContainer<2> args;
    args[0] = leftv;
    args[1] = var;
    _state.invoke("operator=", args);
    return var;
}
Object Evaluator::evalConstant(const Token &token) {
    using TT = Token::Type;
    switch (token.getType()) {
    case TT::INTEGER: {
        auto literal = token.getLiteral();
        int64_t value{};
        std::from_chars(literal.data(), literal.data() + literal.size(), value);
        return Object(value);
    }
    case TT::DECIMAL: {
        auto literal = token.getLiteral();
        double value{};
        std::from_chars(literal.data(), literal.data() + literal.size(), value);
        return Object(value);
    }
    case TT::TRUE_V:
        return Object{true};
    case TT::FALSE_V:
        return Object{false};
    case TT::NIL:
        return {};
    case TT::STRING: {
        auto literal = token.getLiteral();
        if (literal.size() <= 2)
            return Object::create<String>();
        auto str = std::string_view(literal.data() + 1, literal.size() - 2);
        auto obj = Object::create<String>();
        obj.get<String>() = std::move(String(str));
        return obj;
    }
    default:
        NX_ERROR("Error0x0006:Invalid constant token.")
    }
}
Object Evaluator::evalList(const ASTNode *node) {
    auto obj = Object::create<List>();
    auto nodes = node->getNodes();
    for (auto i : nodes) {
        obj.get<List>().append(eval(i));
    }
    return obj;
}
Object Evaluator::evalVar(const Token &token) {
    return _state.getVar(std::string(token.getLiteral()));
}
Object Evaluator::evalPrefix(const ASTNode *node) {
    using TT = Token::Type;
    switch (node->getToken().getType()) {
    case TT::MINUS:
        return OperatorFunc::minus(_state, eval(node->getNodes()[0]));
    case TT::NOT:
        return OperatorFunc::opNot(_state, eval(node->getNodes()[0]));
    default:
        return {};
    }
}
Object Evaluator::evalBinaryOp(const ASTNode *node) {
    using TT = Token::Type;
    switch (node->getToken().getType()) {
    case TT::ASSIGN:
        return evalAssign(node);
    case TT::PLUS:
        return OperatorFunc::plus(_state, eval(node->getNodes()[0]),
                                  eval(node->getNodes()[1]));
    case TT::MINUS:
        return OperatorFunc::minus(_state, eval(node->getNodes()[0]),
                                   eval(node->getNodes()[1]));
    case TT::ASTERISK:
        return OperatorFunc::asterisk(_state, eval(node->getNodes()[0]),
                                      eval(node->getNodes()[1]));
    case TT::SLASH:
        return OperatorFunc::slash(_state, eval(node->getNodes()[0]),
                                   eval(node->getNodes()[1]));
    case TT::POWER:
        return OperatorFunc::power(_state, eval(node->getNodes()[0]),
                                   eval(node->getNodes()[1]));
    case TT::EQUAL:
        return OperatorFunc::equal(_state, eval(node->getNodes()[0]),
                                   eval(node->getNodes()[1]));
    case TT::NOT_EQUAL:
        return OperatorFunc::notEqual(_state, eval(node->getNodes()[0]),
                                      eval(node->getNodes()[1]));
    case TT::GREATER:
        return OperatorFunc::greater(_state, eval(node->getNodes()[0]),
                                     eval(node->getNodes()[1]));
    case TT::GREATER_E:
        return OperatorFunc::greaterEqual(_state, eval(node->getNodes()[0]),
                                          eval(node->getNodes()[1]));
    case TT::LESS:
        return OperatorFunc::less(_state, eval(node->getNodes()[0]),
                                  eval(node->getNodes()[1]));
    case TT::LESS_E:
        return OperatorFunc::lessEqual(_state, eval(node->getNodes()[0]),
                                       eval(node->getNodes()[1]));
    case TT::AND:
        return OperatorFunc::opAnd(_state, eval(node->getNodes()[0]),
                                   eval(node->getNodes()[1]));
    case TT::OR:
        return OperatorFunc::opOr(_state, eval(node->getNodes()[0]),
                                  eval(node->getNodes()[1]));
    case TT::DOT:
    case TT::PIPE:
        return evalFlow(node);
    default:
        return {};
    }
}
Object Evaluator::evalFunction(const ASTNode *node) {
    ArgsContainer args{};
    auto &nodes = node->getNodes();
    args.reserve(nodes.size());
    for (auto item : nodes) {
        args.emplace_back(eval(item));
    }
    std::string name = std::string(node->getToken().getLiteral());
    std::string ns = std::string(node->getToken().getNameSpace());
    return _state.invoke(name, args, ns);
}
Object Evaluator::evalSubscript(const ASTNode *node) {
    ArgsContainer args{};
    auto &nodes = node->getNodes();
    args.reserve(nodes.size() + 1);
    for (auto item : nodes) {
        args.emplace_back(eval(item));
    }
    return OperatorFunc::opSubscript(_state, args);
}
Object Evaluator::evalFlow(const ASTNode *node) {
    using NT = ASTNode::Type;
    auto leftn = node->getNodes()[0];
    auto rightn = node->getNodes()[1];
    if (rightn->match(NT::FUNCTION)) {
        ArgsContainer args{};
        auto &nodes = rightn->getNodes();
        args.reserve(nodes.size() + 1);
        args.emplace_back(eval(leftn));
        for (auto item : nodes) {
            args.emplace_back(eval(item));
        }
        std::string name = std::string(rightn->getToken().getLiteral());
        std::string ns = std::string(rightn->getToken().getNameSpace());
        return _state.invoke(name, args, ns);
    } else if (rightn->match(NT::IDENT)) {
        ArgsFixedContainer<1> args;
        args[0] = eval(leftn);
        std::string name = std::string(rightn->getToken().getLiteral());
        std::string ns = std::string(rightn->getToken().getNameSpace());
        return _state.invoke(name, args, ns);
    }
    NX_ERROR("Error0x0007:No matching function.")
}
} // namespace nx