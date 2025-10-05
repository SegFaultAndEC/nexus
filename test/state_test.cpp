#include "test.hpp"

void stateTest() {
    std::println("---------{} Testing---------", __FUNCTION__);
    nx::State state;
    auto o = nx::Object::create<Foo>();
    state.addVar("obj", o);
    state.addVar("obj", o);
    try {
        nx::ArgsContainer args(1);
        args[0] = nx::Object();
        state.invoke("print", args);
        args[0] = nx::Object(1ll);
        state.invoke("print", args);
        args[0] = nx::Object(1.);
        state.invoke("print", args);
        args[0] = nx::Object(false);
        state.invoke("print", args);
    } catch (const nx::NxError &e) {
        e.show();
    }
}
