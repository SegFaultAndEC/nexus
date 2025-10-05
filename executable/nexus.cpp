#include "error/error.hpp"
#include "evaluator/evaluator.hpp"
#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "types/string.hpp"
#include <iostream>
#include <print>
using namespace nx;

struct CondMark {
    bool exist{false};
    size_t pos{0};
};

void evalLine(std::string_view line, nx::Evaluator &evaluator, nx::State &state,
              int lc = -1) {
    auto tokens = lexer(line);
    bool hasAssign = false;
    CondMark cm;
    size_t exprStart = 0;
    for (size_t i = 0; i < tokens.size(); i++) {
        auto &token = tokens[i];
        switch (token.getType()) {
        case Token::Type::ASSIGN:
            hasAssign = true;
            break;
        case Token::Type::COND_MARK:
            cm = {true, i};
            break;
        default:
            break;
        }
    }

    std::vector<ASTNode *> nodes{};
    auto guarder = PoolCollectGuarder([&] { collectPool(nodes); });
    // 如果存在条件判断
    if (cm.exist) {
        exprStart = cm.pos + 1;
        std::span<Token> condTokens{tokens.data(), cm.pos};
        auto parser = Parser(condTokens, nodes);
        try {
            auto node = parser.parseExpression();
            auto cond = nx::Functions::boolean(state, evaluator.eval(node));
            // 条件不成立，直接退出函数
            if (!(cond.isType<nx::Boolean>() && cond.value.integer != 0)) {
                return;
            }
        } catch (const nx::NxError &e) {
            e.show(lc);
            return;
        }
    }
    std::span<Token> exprTokens{tokens.data() + exprStart,
                                tokens.size() - exprStart};

    auto parser = Parser(exprTokens, nodes);
    try {
        auto node = parser.parseExpression();
        auto rns = evaluator.eval(node);
        if ((!rns.isType<nx::Nil>()) && !(hasAssign)) {
            nx::Functions::write(state, rns);
            std::putchar('\n');
        }
    } catch (const nx::NxError &e) {
        e.show(lc);
    }
}

void evalLines() {
    nx::State state;
    state.loadBuildInFunction();
    nx::Evaluator evaluator(state);
    std::string line;
    while (true) {
        std::print(">>>");
        getline(std::cin, line);
        if (line.empty())
            continue;
        if (line == "exit") {
            break;
        }
        evalLine(line, evaluator, state, 1);
    }
}

int main(int argc, char **args) {
    if (argc == 1) {
        evalLines();
        return 0;
    }
    return 0;
}
