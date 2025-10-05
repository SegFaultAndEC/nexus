#include "test.hpp"

void evalLine(std::string_view line, nx::Evaluator &evaluator, nx::State &state,
              int lc) {
    auto tokens = lexer(line);
    std::vector<ASTNode *> nodes{};
    try {
        auto parser = Parser(tokens, nodes);
        auto node = parser.parseExpression();
        auto rns = evaluator.eval(node);
        nx::ArgsFixedContainer<1> args;
        args[0] = rns;
        nx::Functions::print(state, args);
    } catch (const nx::NxError &e) {
        e.show(lc);
        collectPool(nodes);
    }
}
