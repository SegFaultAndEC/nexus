#include "test.hpp"
#include "types/string.hpp"
#include "types/type.hpp"

void typeTest() {
    std::println("---------{} Testing---------", __FUNCTION__);
    size_t t1{};
    size_t t2 = nx::String::getTypeId();
    std::println("{},{}", nx::Type::isType<nx::String>(t1),
                 nx::Type::isType<nx::String>(t2));
    std::println("integer is value type:{}",
                 nx::Type::isValueT(nx::Integer::getTypeId()));
    std::println("decimal is value type:{}",
                 nx::Type::isValueT(nx::Decimal::getTypeId()));
    std::println("boolean is value type:{}",
                 nx::Type::isValueT(nx::Boolean::getTypeId()));
    std::println("nil is value type:{}",
                 nx::Type::isValueT(nx::Nil::getTypeId()));
    std::println("string is value type:{}",
                 nx::Type::isValueT(nx::String::getTypeId()));
}