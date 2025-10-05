#include "state.hpp"
#include "error/error.hpp"
#include <stdexcept>

namespace nx {
void State::loadBuildInFunction() {
    Functions::registerBuildInFunctions(*this);
}
Object State::getVar(const std::string &name) {
    auto iter = _variables.find(name);
    if (iter == _variables.end())
        NX_ERROR("Error0x0003:Undefined identifier '{}'.", name)
    return iter->second;
}
Object State::invoke(const std::string &name, ArgsT args,
                     const std::string &nameSpace) {
    if (nameSpace.empty()) {
        auto iter = _functions.find(name);
        if (iter == _functions.end())
            NX_ERROR("Error0x0007:No matching function.")
        return iter->second.invoke(*this, args);
    }
    auto namesIter = _funcWithNamespace.find(nameSpace);
    if (namesIter == _funcWithNamespace.end())
        NX_ERROR("Error0x0007:No matching function.")
    auto funcIter = namesIter->second.find(name);
    if (funcIter == namesIter->second.end())
        NX_ERROR("Error0x0007:No matching function.")
    return funcIter->second.invoke(*this, args);
}
void State::addVar(const std::string &name, const Object &obj) {
    _variables[name] = obj;
}
void State::addVar(const std::string &name, Object &&obj) {
    _variables[name] = std::move(obj);
}
void State::addFunction(const std::string &name, Function func,
                        const std::string &nameSpace) {
    if (nameSpace.empty()) {
        _functions[name].addFunction(func);
        return;
    }
    _funcWithNamespace[nameSpace][name].addFunction(func);
}
bool State::hasVar(const std::string &name) const {
    return _variables.find(name) != _variables.end();
}
bool State::hasFunction(const std::string &name,
                        const std::string &nameSpace) const {
    if (nameSpace.empty())
        return _functions.find(name) != _functions.end();
    auto iter = _funcWithNamespace.find(nameSpace);
    if (iter != _funcWithNamespace.end())
        return iter->second.find(name) != iter->second.end();
    return false;
}
bool State::hasNameSpace(const std::string &name) const {
    return _funcWithNamespace.find(name) != _funcWithNamespace.end();
}
} // namespace nx