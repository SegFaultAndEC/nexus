#include "test.hpp"
using namespace nx;

void evalLine(std::string_view line, nx::Evaluator &evaluator, nx::State &state,
              int lc = -1) {
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

#define EVAL_TEST_LINE(line) evalLine(line, evaluator, state, 1);
#define NOT_INPUT_TEST return;

void evalTest() {
    std::println("---------{} Testing---------", __FUNCTION__);
    nx::State state;
    state.loadBuildInFunction();
    nx::Evaluator evaluator(state);

    EVAL_TEST_LINE("s=\"abcdef\"")
    EVAL_TEST_LINE("s[1]")
    NOT_INPUT_TEST
}