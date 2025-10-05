#ifndef NEXUS_TEST_HPP
#define NEXUS_TEST_HPP

#include "error/error.hpp"
#include "evaluator/evaluator.hpp"
#include "extension/extension.hpp"
#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "state/state.hpp"
#include "types/object.hpp"
#include "types/string.hpp"
#include <iostream>
#include <print>
using namespace std;
using namespace nx;

void lexerTest();
void parserTest();
void typeTest();
void objectTest();
void stateTest();
void evalTest();
void stringTest();
void importTest();

struct Foo {
    NX_OBJECT(Foo)
  public:
    Foo() = default;
    Foo(const Foo &f) { data = f.data; }
    ~Foo() { println("Foo destruction"); }
    void show() { println("{}", data); }
    static void *nxTypeFunc(nx::TypeFuncAction action, void *ptr = nullptr) {
        switch (action) {
        case nx::TypeFuncAction::ALLOCATE:
            return new Foo();
        case nx::TypeFuncAction::DEALLOCATE: {
            delete reinterpret_cast<Foo *>(ptr);
            break;
        }
        case nx::TypeFuncAction::COPY:
            return new Foo(*reinterpret_cast<Foo *>(ptr));
        }
        return nullptr;
    }
    int data{};
};

static void showAST(ASTNode *node, const std::string &prefix = "",
                    bool isLast = true) {
    if (!node)
        return;

    // 打印前缀和分支符号
    std::cout << prefix;
    if (isLast) {
        std::cout << "|__ ";
    } else {
        std::cout << "|-- ";
    }

    // 打印节点内容
    std::cout << "(";
    switch (node->getType()) {
    case ASTNode::Type::CONSTANT:
        std::cout << "CONST";
        break;
    case ASTNode::Type::IDENT:
        std::cout << "IDENT";
        break;
    case ASTNode::Type::FUNCTION:
        std::cout << "FUNC";
        break;
    case ASTNode::Type::PREFIX:
        std::cout << "PREFIX";
        break;
    case ASTNode::Type::BINARY:
        std::cout << "INFIX";
        break;
    default:
        std::cout << "UNKNOWN";
        break;
    }
    std::cout << ", " << node->getToken().getLiteral() << ")\n";

    // 子节点递归
    const auto &children = node->getNodes();
    for (size_t i = 0; i < children.size(); i++) {
        bool last = (i == children.size() - 1);
        showAST(children[i], prefix + (isLast ? "    " : "|   "), last);
    }
}
#endif // NEXUS_TEST_HPP
