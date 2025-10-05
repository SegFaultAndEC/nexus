#ifndef NEXUS_TYPE_HPP
#define NEXUS_TYPE_HPP

#define NX_EMPTY_TYPE_FUNC(T)                                                  \
  public:                                                                      \
    static void *nxTypeFunc(::nx::TypeFuncAction action,                       \
                            void *ptr = nullptr) {                             \
        return nullptr;                                                        \
    }

#define NX_OBJECT(T)                                                           \
  private:                                                                     \
    static inline T *typeId;                                                   \
                                                                               \
  public:                                                                      \
    static size_t getTypeId() { return reinterpret_cast<size_t>(&T::typeId); }

namespace nx {
enum class TypeFuncAction {
    ALLOCATE = 0, // 分配
    DEALLOCATE,   // 删除
    COPY,         // 复制
};

using TypeFunc = void *(*)(TypeFuncAction, void *);

struct Integer {
    NX_OBJECT(Integer)
    NX_EMPTY_TYPE_FUNC(Integer)
};
struct Decimal {
    NX_OBJECT(Decimal)
    NX_EMPTY_TYPE_FUNC(Decimal)
};
struct Character {
    NX_OBJECT(Character)
    NX_EMPTY_TYPE_FUNC(Character)
};
struct Boolean {
    NX_OBJECT(Boolean)
    NX_EMPTY_TYPE_FUNC(Boolean)
};
struct Nil {
    NX_OBJECT(Nil)
    NX_EMPTY_TYPE_FUNC(Nil)
};

struct Type {
    template <class T> static bool isType(size_t typeId) {
        return typeId == T::getTypeId();
    }

    static bool isValueT(size_t typeId) {
        return typeId == Integer::getTypeId() ||
               typeId == Decimal::getTypeId() ||
               typeId == Character::getTypeId() ||
               typeId == Boolean::getTypeId() || typeId == Nil::getTypeId();
    }
};
} // namespace nx

#endif // NEXUS_TYPE_HPP
