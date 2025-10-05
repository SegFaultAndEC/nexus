#ifndef NEXUS_STATE_HPP
#define NEXUS_STATE_HPP

#include "function/function.hpp"
#include "types/object.hpp"
#include <string>
#include <unordered_map>

namespace nx {
class State {
    using FNListT = std::unordered_map<std::string, FunctionList>;

  public:
    void loadBuildInFunction();

    Object getVar(const std::string &name);
    Object invoke(const std::string &name, ArgsT args,
                  const std::string &nameSpace = "");

    void addVar(const std::string &name, const Object &obj);
    void addVar(const std::string &name, Object &&obj);
    void addFunction(const std::string &name, Function func,
                     const std::string &nameSpace = "");

    bool hasVar(const std::string &name) const;
    bool hasFunction(const std::string &name,
                     const std::string &nameSpace = "") const;
    bool hasNameSpace(const std::string &name) const;

  private:
    Object _self{};
    std::unordered_map<std::string, Object> _variables{};
    FNListT _functions{};
    std::unordered_map<std::string, FNListT> _funcWithNamespace{};
};
} // namespace nx

#endif // NEXUS_STATE_HPP
