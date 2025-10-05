#include "extension/extension.hpp"

auto func(nx::State &state, nx::ArgsT) -> nx::ReturnVT {
    std::println("hello");
    NX_ACTION_BREAK
};

NX_REGISTER_BEGIN
state.addFunction("func", func, "std");
NX_REGISTER_END