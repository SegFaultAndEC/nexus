#ifndef NEXUS_OBJECT_HPP
#define NEXUS_OBJECT_HPP
#include "type.hpp"
#include <concepts>
#include <cstdint>

namespace nx {
struct Object {
    union Value {
        void *ptr{};
        int64_t integer;
        double decimal;
        char32_t character;
    };
    Object();
    ~Object();
    explicit Object(int64_t integer);
    explicit Object(double decimal);
    explicit Object(bool boolean);
    explicit Object(char32_t character);
    Object(const Object &obj);
    Object(Object &&obj) noexcept;
    template <class T> static Object create() {
        Object obj;
        if (std::is_same_v<T, Integer>) {
            obj._func = Integer::nxTypeFunc;
            return obj;
        } else if (std::is_same_v<T, Decimal>) {
            obj._func = Decimal::nxTypeFunc;
            return obj;
        } else if (std::is_same_v<T, Boolean>) {
            obj._func = Boolean::nxTypeFunc;
            return obj;
        } else if (std::is_same_v<T, Nil>) {
            obj._func = Nil::nxTypeFunc;
            return obj;
        }

        obj.value.ptr = T::nxTypeFunc(TypeFuncAction::ALLOCATE);
        obj._func = T::nxTypeFunc;
        obj._typeId = T::getTypeId();
        obj._refCount = new uint32_t(1);
        return obj;
    }
    template <class T> bool isType() const { return Type::isType<T>(_typeId); }
    bool isValueT() const;
    // 是否为右值对象
    bool isRValue() const;

    Object &operator=(const Object &obj);
    Object &operator=(Object &&obj) noexcept;

    Object copy() const;
    void release();

    template <class T> T &get() const {
        return *reinterpret_cast<T *>(value.ptr);
    }

    Value value{};

  private:
    Object copyValue() const;
    TypeFunc _func{};
    size_t _typeId{};
    uint32_t *_refCount{};
};
} // namespace nx

#endif // NEXUS_OBJECT_HPP
