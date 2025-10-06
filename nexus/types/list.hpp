#ifndef NEXUS_LIST_HPP
#define NEXUS_LIST_HPP

#include "error/error.hpp"
#include "object.hpp"
#include "type.hpp"
#include <vector>

namespace nx {
class List {
    NX_OBJECT(List)
  public:
    List() = default;
    List(const List &list);
    List(List &&list) noexcept;
    List &operator=(const List &list);
    List &operator=(List &&list) noexcept;

    static void *nxTypeFunc(TypeFuncAction action, void *ptr = nullptr);

    void append(const Object &obj);
    void append(const List &list);
    Object &at(int64_t index);
    void set(int64_t index, const Object &obj);
    int64_t size();

  private:
    std::vector<Object> _list{};
};
} // namespace nx

#endif // NEXUS_LIST_HPP
