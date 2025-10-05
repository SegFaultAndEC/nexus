#include "test.hpp"

void func(nx::Object obj) {}

void objectTest() {
    std::println("---------{} Testing---------", __FUNCTION__);
    nx::Object obj = nx::Object::create<Foo>();
    func(obj);
    auto obj2 = obj;
    println("1");
    println("{}", obj.value.ptr);
    println("{}", obj2.value.ptr);
}