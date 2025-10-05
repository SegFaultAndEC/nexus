#include "test.hpp"
using namespace std;

#define LEXER_TEST_LINE(title, line)                                           \
    std::println(title);                                                       \
    for (auto &item : lexer(line)) {                                           \
        if (item.nsEmpty())                                                    \
            println("type:{},literal:{}", Token::getName(item.getType()),      \
                    item.getLiteral());                                        \
        else                                                                   \
            println("type:{},namespace:{},literal:{}",                         \
                    Token::getName(item.getType()), item.getNameSpace(),       \
                    item.getLiteral());                                        \
    }                                                                          \
    cout << endl;

void lexerTest() {
    std::println("---------{} Testing---------", __FUNCTION__);
    LEXER_TEST_LINE("common test", "(1+2)-3.5*3./5363")
    LEXER_TEST_LINE("namespace test", "a.a->b")
    LEXER_TEST_LINE("complex Test",
                    "\"a\"|((not(flag))or true)and(c^1^2)|a.func(arg)->i")

    cout << endl;
}
