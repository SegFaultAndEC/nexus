#include "operator_func.hpp"
#include "error/error.hpp"
#include "types/list.hpp"
#include "types/string.hpp"
namespace nx {
static bool toBool(const Object &obj) {
    if (obj.isType<Nil>())
        return false;
    else if (obj.isType<Integer>())
        return obj.value.integer != 0;
    else if (obj.isType<Decimal>())
        return obj.value.decimal != 0;
    else if (obj.isType<Character>())
        return obj.value.character != U'\0';
    else if (obj.isType<Boolean>())
        return obj.value.integer;
    return false;
}
static double toDecimal(const Object &obj) {
    if (obj.isType<Nil>())
        return 0.;
    else if (obj.isType<Integer>() || obj.isType<Boolean>())
        return static_cast<double>(obj.value.integer);
    else if (obj.isType<Character>())
        return static_cast<double>(obj.value.character);
    else if (obj.isType<Decimal>())
        return obj.value.decimal;
    return 0.;
}
static int64_t toInteger(const Object &obj) {
    if (obj.isType<Nil>())
        return 0;
    else if (obj.isType<Integer>() || obj.isType<Boolean>())
        return obj.value.integer;
    else if (obj.isType<Character>())
        return static_cast<int64_t>(obj.value.character);
    else if (obj.isType<Decimal>())
        return static_cast<int64_t>(obj.value.decimal);
    return 0;
}

Object OperatorFunc::plus(State &state, const Object &a, const Object &b) {
    // 字符串和字符的拼接
    if (a.isType<String>() && b.isType<String>()) {
        Object obj = a.copy();
        obj.get<String>().append(b.get<String>());
        return obj;
    }
    if (a.isType<String>() && b.isType<Character>()) {
        Object obj = a.copy();
        obj.get<String>().append(b.value.character);
        return obj;
    }
    // 列表拼接
    if (a.isType<List>() && b.isType<List>()) {
        Object obj = a.copy();
        obj.get<List>().append(b.get<List>());
        return obj;
    }
    if (a.isType<List>()) {
        Object obj = a.copy();
        obj.get<List>().append(b);
        return obj;
    }
    if (!(a.isValueT() && b.isValueT())) {
        ArgsFixedContainer<2> args;
        args[0] = a;
        args[1] = b;
        return state.invoke("operator+", args);
    }

    if (a.isType<Nil>())
        return b;
    else if (b.isType<Nil>())
        return a;

    if (a.isType<Character>() && b.isType<Character>()) {
        char32_t res = a.value.character + b.value.character;
        return Object{res};
    } else if (a.isType<Character>() && b.isType<Integer>()) {
        char32_t res = a.value.character + b.value.integer;
        return Object{res};
    } else if (a.isType<Integer>() && b.isType<Character>()) {
        char32_t res = a.value.integer + b.value.character;
        return Object{res};
    }

    if (a.isType<Decimal>() || b.isType<Decimal>()) {
        auto left = toDecimal(a);
        auto right = toDecimal(b);
        return Object{left + right};
    } else {
        auto left = toInteger(a);
        auto right = toInteger(b);
        return Object{left + right};
    }
}
Object OperatorFunc::minus(State &state, const Object &obj) {
    if (!obj.isValueT()) {
        ArgsFixedContainer<1> args;
        args[0] = obj;
        return state.invoke("operator-", args);
    }

    if (obj.isType<Nil>())
        return obj;
    else if (obj.isType<Decimal>())
        return Object{-obj.value.decimal};
    else if (obj.isType<Character>()) {
        char32_t res = -obj.value.character;
        return Object{res};
    } else if (obj.isType<Integer>() || obj.isType<Boolean>())
        return Object{-obj.value.integer};
    else
        return {};
}
Object OperatorFunc::minus(State &state, const Object &a, const Object &b) {
    if (!(a.isValueT() && b.isValueT())) {
        ArgsFixedContainer<2> args;
        args[0] = a;
        args[1] = b;
        return state.invoke("operator-", args);
    }

    if (a.isType<Nil>())
        return b;
    else if (b.isType<Nil>())
        return a;

    if (a.isType<Character>() && b.isType<Character>()) {
        char32_t res = a.value.character - b.value.character;
        return Object{res};
    } else if (a.isType<Character>()) {
        char32_t res = a.value.character - b.value.integer;
        return Object{res};
    } else if (b.isType<Character>()) {
        char32_t res = a.value.integer - b.value.character;
        return Object{res};
    }
    if (a.isType<Decimal>() || b.isType<Decimal>()) {
        auto left = toDecimal(a);
        auto right = toDecimal(b);
        return Object{left - right};
    } else {
        auto left = toInteger(a);
        auto right = toInteger(b);
        return Object{left - right};
    }
}
Object OperatorFunc::asterisk(State &state, const Object &a, const Object &b) {
    if (!(a.isValueT() && b.isValueT())) {
        ArgsFixedContainer<2> args;
        args[0] = a;
        args[1] = b;
        return state.invoke("operator*", args);
    }

    if (a.isType<Nil>() || b.isType<Nil>())
        return {};

    if (a.isType<Decimal>() || b.isType<Decimal>()) {
        auto left = toDecimal(a);
        auto right = toDecimal(b);
        return Object{left * right};
    } else {
        auto left = toInteger(a);
        auto right = toInteger(b);
        return Object{left * right};
    }
}
Object OperatorFunc::slash(State &state, const Object &a, const Object &b) {
    if (!(a.isValueT() && b.isValueT())) {
        ArgsFixedContainer<2> args;
        args[0] = a;
        args[1] = b;
        return state.invoke("operator/", args);
    }
    if (a.isType<Nil>())
        return {};

    auto left = toDecimal(a);
    auto right = toDecimal(b);

    if (right == 0)
        NX_ERROR("Error0x0004:Division by zero or null is undefined.")
    return Object{left / right};
}
Object OperatorFunc::power(State &state, const Object &a, const Object &b) {
    if (!(a.isValueT() && b.isValueT())) {
        ArgsFixedContainer<2> args;
        args[0] = a;
        args[1] = b;
        return state.invoke("operator^", args);
    }

    if (a.isType<Nil>())
        return {};

    auto left = toDecimal(a);
    auto right = toDecimal(b);
    return Object{std::pow(left, right)};
}
Object OperatorFunc::equal(State &state, const Object &a, const Object &b) {
    if (!(a.isValueT() && b.isValueT())) {
        ArgsFixedContainer<2> args;
        args[0] = a;
        args[1] = b;
        return state.invoke("operator==", args);
    }

    if (a.isType<Nil>() && b.isType<Nil>()) {
        return Object{true};
    } else if (a.isType<Nil>() || b.isType<Nil>()) {
        return Object{false};
    }

    if (a.isType<Decimal>() || b.isType<Decimal>()) {
        auto left = toDecimal(a);
        auto right = toDecimal(b);
        return Object{left == right};
    } else {
        auto left = toInteger(a);
        auto right = toInteger(b);
        return Object{left == right};
    }
}
Object OperatorFunc::notEqual(State &state, const Object &a, const Object &b) {
    if (!(a.isValueT() && b.isValueT())) {
        ArgsFixedContainer<2> args;
        args[0] = a;
        args[1] = b;
        return state.invoke("operator!=", args);
    }

    if (a.isType<Nil>() && b.isType<Nil>()) {
        return Object{false};
    } else if (a.isType<Nil>() || b.isType<Nil>()) {
        return Object{true};
    }

    if (a.isType<Decimal>() || b.isType<Decimal>()) {
        auto left = toDecimal(a);
        auto right = toDecimal(b);
        return Object{left != right};
    } else {
        auto left = toInteger(a);
        auto right = toInteger(b);
        return Object{left != right};
    }
}
Object OperatorFunc::greater(State &state, const Object &a, const Object &b) {
    if (!(a.isValueT() && b.isValueT())) {
        ArgsFixedContainer<2> args;
        args[0] = a;
        args[1] = b;
        return state.invoke("operator>", args);
    }

    if (a.isType<Nil>())
        return Object{false};
    else if (b.isType<Nil>())
        return Object{true};

    if (a.isType<Decimal>() || b.isType<Decimal>()) {
        auto left = toDecimal(a);
        auto right = toDecimal(b);
        return Object{left > right};
    } else {
        auto left = toInteger(a);
        auto right = toInteger(b);
        return Object{left > right};
    }
}
Object OperatorFunc::greaterEqual(State &state, const Object &a,
                                  const Object &b) {
    if (!(a.isValueT() && b.isValueT())) {

        ArgsFixedContainer<2> args;
        args[0] = a;
        args[1] = b;
        return state.invoke("operator>=", args);
    }

    if (b.isType<Nil>())
        return Object{true};
    else if (a.isType<Nil>())
        return Object{false};

    if (a.isType<Decimal>() || b.isType<Decimal>()) {
        auto left = toDecimal(a);
        auto right = toDecimal(b);
        return Object{left >= right};
    } else {
        auto left = toInteger(a);
        auto right = toInteger(b);
        return Object{left >= right};
    }
}
Object OperatorFunc::less(State &state, const Object &a, const Object &b) {
    if (!(a.isValueT() && b.isValueT())) {
        ArgsFixedContainer<2> args;
        args[0] = a;
        args[1] = b;
        return state.invoke("operator<", args);
    }

    if (b.isType<Nil>())
        return Object{false};
    else if (a.isType<Nil>())
        return Object{true};

    if (a.isType<Decimal>() || b.isType<Decimal>()) {
        auto left = toDecimal(a);
        auto right = toDecimal(b);
        return Object{left < right};
    } else {
        auto left = toInteger(a);
        auto right = toInteger(b);
        return Object{left < right};
    }
}
Object OperatorFunc::lessEqual(State &state, const Object &a, const Object &b) {
    if (!(a.isValueT() && b.isValueT())) {
        ArgsFixedContainer<2> args;
        args[0] = a;
        args[1] = b;
        return state.invoke("operator<=", args);
    }

    if (a.isType<Nil>())
        return Object{true};
    else if (b.isType<Nil>())
        return Object{false};

    if (a.isType<Decimal>() || b.isType<Decimal>()) {
        auto left = toDecimal(a);
        auto right = toDecimal(b);
        return Object{left <= right};
    } else {
        auto left = toInteger(a);
        auto right = toInteger(b);
        return Object{left <= right};
    }
}

Object OperatorFunc::opAnd(State &state, const Object &a, const Object &b) {
    if (!(a.isValueT() && b.isValueT())) {
        ArgsFixedContainer<2> args;
        args[0] = a;
        args[1] = b;
        return state.invoke("operator@and", args);
    }
    bool left = toBool(a);
    bool right = toBool(b);
    return Object{left && right};
}
Object OperatorFunc::opOr(State &state, const Object &a, const Object &b) {
    if (!(a.isValueT() && b.isValueT())) {
        ArgsFixedContainer<2> args;
        args[0] = a;
        args[1] = b;
        return state.invoke("operator@or", args);
    }
    bool left = toBool(a);
    bool right = toBool(b);
    return Object{left || right};
}
Object OperatorFunc::opNot(State &state, const Object &obj) {
    if (!obj.isValueT()) {
        ArgsFixedContainer<1> args;
        args[0] = obj;
        return state.invoke("operator@not", args);
    }
    bool var = toBool(obj);
    return Object{!var};
}
Object OperatorFunc::opSubscript(State &state, ArgsT args) {
    if (args.size() == 2) {
        if (args[0].isType<String>() && args[1].isType<Integer>())
            return Object{args[0].get<String>().at(args[1].value.integer)};
        if (args[0].isType<List>() && args[1].isType<Integer>())
            return Object{args[0].get<List>().at(args[1].value.integer)};
    }
    return state.invoke("operator[]", args);
}
} // namespace nx