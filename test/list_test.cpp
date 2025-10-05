#include "test.hpp"

void listTest() {
    TEST_BEGIN
    EVAL_TEST_LINE("l=['abc',1,true,false]")
    EVAL_TEST_LINE("l")
    EVAL_TEST_LINE("l[0][0]")
    EVAL_TEST_LINE("l[0]=2")
    EVAL_TEST_LINE("l")
}