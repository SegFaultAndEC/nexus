#include "test.hpp"

void parserLine(const string &line) {
    auto tokens = lexer(line);
    std::vector<ASTNode *> nodes{};
    auto parser = Parser(tokens, nodes);
    auto node = parser.parseExpression();
    showAST(node);
    collectPool(nodes);
}
#define TEST_LINE(line)                                                        \
    std::println("---line{}---", num);                                         \
    parserLine(line);                                                          \
    num++;

void parserTest() {
    std::println("---------{} Testing---------", __FUNCTION__);
    int num = 1;
    TEST_LINE("1+2")
    TEST_LINE("-9+6")
    TEST_LINE("((((((1+1)))+1+1)))")
    TEST_LINE("1+1*5/(7+11+(3-99*7))")
    TEST_LINE("a.b.c.d.e.f.g")
    try {
        // error
        TEST_LINE("1..func")
    } catch (const nx::NxError &e) {
        e.show();
    }
    TEST_LINE("1+a.func(1+1,2+2,456)*2+b.c|i")
}