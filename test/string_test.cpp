
#include "test.hpp"
void stringTest() {
    string_view sv = "hhhh";
    string_view sv1 = "h你好";
    string_view sv2 = "h你好😺";
    nx::String s = std::move(nx::String(sv2));
    std::println("{}", sv2);
    std::println("{}", s.toStdString());
    std::cout << sv2 << std::endl;
}
