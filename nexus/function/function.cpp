#include "function.hpp"
#include "error/error.hpp"
#include "state/state.hpp"
#include "types/list.hpp"
#include "types/string.hpp"
#include <iostream>

namespace nx {
ReturnVT Functions::print(State &state, ArgsT args) {
    for (auto &item : args) {
        if (item.isType<String>())
            std::cout << item.get<String>().toStdString();
        else
            write(state, item);
    }
    std::putchar('\n');
    NX_ACTION_BREAK
}
ReturnVT Functions::printr(State &state, ArgsT args) {
    for (auto &item : args) {
        write(state, item);
    }
    NX_ACTION_BREAK
}
ReturnVT Functions::at(State &state, ArgsT args) {
    if (args.size() == 2) {
        if (args[0].isType<String>() && args[1].isType<Integer>()) {
            NX_FUNCTION_BREAK(
                Object{args[0].get<String>().at(args[1].value.integer)})
        }
        if (args[0].isType<List>() && args[1].isType<Integer>()) {
            NX_FUNCTION_BREAK(
                Object{args[0].get<List>().at(args[1].value.integer)})
        }
    }
    NX_CONTINUE
}
void Functions::registerBuildInFunctions(State &state) {
    state.addFunction("print", Functions::print);
    state.addFunction("printr", Functions::printr);
    state.addFunction("at", Functions::at);
}
void Functions::write(State &state, const Object &obj) {
    if (obj.isType<Integer>()) {
        std::cout << obj.value.integer;
        return;
    }
    if (obj.isType<Decimal>()) {
        std::cout << obj.value.decimal;
        return;
    }
    if (obj.isType<Character>()) {
        std::cout << '\'' << String::char32ToChar8(obj.value.character).data()
                  << '\'';
        return;
    }
    if (obj.isType<Boolean>()) {
        if (obj.value.integer)
            std::cout << "true";
        else
            std::cout << "false";
        return;
    }
    if (obj.isType<Nil>()) {
        std::cout << "nil";
        return;
    }
    if (obj.isType<String>()) {
        std::cout << '\'' << obj.get<String>().toStdString() << '\'';
        return;
    }
    if (obj.isType<List>()) {
        auto list = obj.get<List>();
        std::putchar('[');
        for (int64_t i = 0; i < list.size(); i++) {
            write(state, list.at(i));
            if (i + 1 != list.size()) {
                std::putchar(',');
                std::putchar(' ');
            }
        }
        std::putchar(']');
        return;
    }
    ArgsFixedContainer<1> args;
    args[0] = obj;
    state.invoke("@write", args);
}
Object Functions::boolean(State &state, const Object &obj) {
    if (obj.isType<Nil>())
        return Object{false};
    else if (obj.isType<Integer>())
        return Object{obj.value.integer != 0};
    else if (obj.isType<Decimal>())
        return Object{obj.value.decimal != 0};
    else if (obj.isType<Character>())
        return Object{obj.value.character != U'\0'};
    else if (obj.isType<Boolean>())
        return obj;

    ArgsFixedContainer<1> args;
    args[0] = obj;
    return state.invoke("@boolean", args);
}
Object Functions::decimal(State &state, const Object &obj) {
    if (obj.isType<Nil>())
        return Object{0.};
    else if (obj.isType<Integer>() || obj.isType<Boolean>())
        return Object{static_cast<double>(obj.value.integer)};
    else if (obj.isType<Character>())
        return Object{static_cast<double>(obj.value.character)};
    else if (obj.isType<Decimal>())
        return obj;
    ArgsFixedContainer<1> args;
    args[0] = obj;
    return state.invoke("@decimal", args);
}
Object Functions::integer(State &state, const Object &obj) {
    if (obj.isType<Nil>())
        return Object{0ll};
    else if (obj.isType<Integer>() || obj.isType<Boolean>())
        return obj;
    else if (obj.isType<Character>())
        return Object{static_cast<int64_t>(obj.value.character)};
    else if (obj.isType<Decimal>())
        return Object{static_cast<int64_t>(obj.value.decimal)};
    ArgsFixedContainer<1> args;
    args[0] = obj;
    return state.invoke("@integer", args);
}
FunctionList::FunctionList() = default;
Object FunctionList::invoke(State &state, ArgsT args) {
    // 若列表中无任何函数
    if (!_first)
        NX_ERROR("Error0x0007:No matching function.")
    auto ret = _first(state, args);
    if (ret.finished)
        return ret.obj;
    for (auto func : _list) {
        ret = func(state, args);
        if (ret.finished)
            return ret.obj;
    }
    NX_ERROR("Error0x0007:No matching function.")
}
void FunctionList::addFunction(Function func) {
    if (!_first)
        _first = func;
    else
        _list.emplace_back(func);
}
} // namespace nx
