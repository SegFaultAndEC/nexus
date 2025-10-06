#include "state/state.hpp"
#include "types/object.hpp"
namespace nx {
struct OperatorFunc {
    static Object plus(State &state, const Object &a, const Object &b);
    static Object minus(State &state, const Object &obj);
    static Object minus(State &state, const Object &a, const Object &b);
    static Object asterisk(State &state, const Object &a, const Object &b);
    static Object slash(State &state, const Object &a, const Object &b);
    static Object power(State &state, const Object &a, const Object &b);

    static Object equal(State &state, const Object &a, const Object &b);
    static Object notEqual(State &state, const Object &a, const Object &b);
    static Object greater(State &state, const Object &a, const Object &b);
    static Object greaterEqual(State &state, const Object &a, const Object &b);
    static Object less(State &state, const Object &a, const Object &b);
    static Object lessEqual(State &state, const Object &a, const Object &b);

    static Object opAnd(State &state, const Object &a, const Object &b);
    static Object opOr(State &state, const Object &a, const Object &b);
    static Object opNot(State &state, const Object &obj);

    static Object opIndex(State &state, ArgsT args);
    static Object opIndexAssign(State &state, ArgsT args);
};
} // namespace nx