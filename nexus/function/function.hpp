#ifndef NEXUS_FUNCTION_HPP
#define NEXUS_FUNCTION_HPP
#include "types/object.hpp"
#include <array>
#include <span>
#include <string>
#include <vector>

#define NX_ACTION_BREAK return ::nx::ReturnVT{true, {}};
#define NX_FUNCTION_BREAK(val) return ::nx::ReturnVT{true, val};
#define NX_CONTINUE return ::nx::ReturnVT{false, {}};

namespace nx {
class State;

struct ReturnVT {
    bool finished{false};
    Object obj{};
    bool isNil() { return obj.isType<Nil>(); }
};
using ArgsContainer = std::vector<Object>;
template <size_t size> using ArgsFixedContainer = std::array<Object, size>;

using ArgsT = std::span<Object>;
using Function = ReturnVT (*)(State &state, ArgsT);

class FunctionList {
    using ListT = std::vector<Function>;

  public:
    FunctionList();
    Object invoke(State &state, ArgsT args);
    void addFunction(Function func);

  private:
    Function _first{};
    ListT _list{};
};

// 内置函数
struct Functions {
    // 需要注册进状态机的函数
    static ReturnVT print(State &state, ArgsT args);
    static ReturnVT printr(State &state, ArgsT args);
    static ReturnVT at(State &state, ArgsT args);

    static void registerBuildInFunctions(State &state);

    // 可重载内置函数的原始形式
    static void write(State &state, const Object &obj);
    static Object boolean(State &state, const Object &obj);
    static Object decimal(State &state, const Object &obj);
    static Object integer(State &state, const Object &obj);
};
} // namespace nx

#endif // NEXUS_FUNCTION_HPP
