#ifdef _WIN32
#include <windows.h>
#endif

#include "test.hpp"

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    lexerTest();
    parserTest();
    typeTest();
    objectTest();
    stateTest();
    evalTest();
    stringTest();
    namespaceTest();
    listTest();

    return 0;
}
