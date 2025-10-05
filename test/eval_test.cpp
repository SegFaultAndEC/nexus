#include "test.hpp"
using namespace nx;

void evalTest() {
    TEST_BEGIN
    EVAL_TEST_LINE("1+1")
    EVAL_TEST_LINE("(3).print")
}