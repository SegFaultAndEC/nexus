#include "test.hpp"

void stringTest() {
    TEST_BEGIN
    EVAL_TEST_LINE("s=\"你好😺\"")
    EVAL_TEST_LINE("s.print")
    EVAL_TEST_LINE("s=\"hello\"")
    EVAL_TEST_LINE("s+s[0]")
}
