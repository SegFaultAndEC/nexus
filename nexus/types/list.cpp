#include "list.hpp"
namespace nx {

List::List(const nx::List &list) : _list(list._list) {}
List::List(List &&list) noexcept : _list(std::move(list._list)) {
    list._list.clear();
}
List &List::operator=(const List &list) {
    if (this != &list) {
        _list = list._list;
    }
    return *this;
}
List &List::operator=(List &&list) noexcept {
    if (this != &list) {
        _list = std::move(list._list);
        list._list.clear();
    }
    return *this;
}
void *List::nxTypeFunc(TypeFuncAction action, void *ptr) {
    switch (action) {
    case TypeFuncAction::ALLOCATE:
        return new List();
    case TypeFuncAction::DEALLOCATE: {
        delete reinterpret_cast<List *>(ptr);
        break;
    }
    case TypeFuncAction::COPY:
        return new List(*reinterpret_cast<List *>(ptr));
    }
    return nullptr;
}
void List::append(const Object &obj) { _list.push_back(obj); }
void List::append(const List &list) {
    _list.insert(_list.end(), list._list.begin(), list._list.end());
}
Object &List::at(int64_t index) {
    size_t indexs = static_cast<size_t>(index);
    if (indexs >= size()) {
        NX_ERROR("Error0x0010:Index out of range.");
    }
    return _list[index];
}
int64_t List::size() { return static_cast<int64_t>(_list.size()); }
void List::set(int64_t index, const Object &obj) {
    size_t indexs = static_cast<size_t>(index);
    _list[indexs] = obj;
}
} // namespace nx
