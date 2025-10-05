#include "object.hpp"

namespace nx {
Object::Object() : _func(Nil::nxTypeFunc), _typeId(Nil::getTypeId()) {}
Object::Object(int64_t integer)
    : _func(Integer::nxTypeFunc), _typeId(Integer::getTypeId()) {
    this->value.integer = integer;
}
Object::Object(double decimal)
    : _func(Decimal::nxTypeFunc), _typeId(Decimal::getTypeId()) {
    this->value.decimal = decimal;
}
Object::Object(char32_t character)
    : _func(Character::nxTypeFunc), _typeId(Character::getTypeId()) {
    this->value.character = character;
}
Object::Object(bool boolean)
    : _func(Boolean::nxTypeFunc), _typeId(Boolean::getTypeId()) {
    this->value.integer = boolean;
}
Object::Object(const Object &obj) {
    value = obj.value;
    _func = obj._func;
    _typeId = obj._typeId;
    _refCount = obj._refCount;
    if (_refCount)
        ++(*_refCount);
}
Object::Object(Object &&obj) noexcept
    : value(obj.value), _func(obj._func), _typeId(obj._typeId),
      _refCount(obj._refCount) {
    obj.value.ptr = nullptr;
    obj._func = nullptr;
    obj._typeId = Nil::getTypeId();
    obj._refCount = nullptr;
}
Object::~Object() { release(); }

Object &Object::operator=(const Object &obj) {
    if (this == &obj || (value.ptr == obj.value.ptr && _typeId == obj._typeId))
        return *this;
    release();
    value = obj.value;
    _func = obj._func;
    _typeId = obj._typeId;
    _refCount = obj._refCount;
    if (_refCount)
        ++(*_refCount);
    return *this;
}

Object &Object::operator=(Object &&obj) noexcept {
    if (this == &obj || (value.ptr == obj.value.ptr && _typeId == obj._typeId))
        return *this;
    release();

    value = obj.value;
    _func = obj._func;
    _typeId = obj._typeId;
    _refCount = obj._refCount;

    obj.value.ptr = nullptr;
    obj._refCount = nullptr;
    obj._typeId = Nil::getTypeId();
    obj._func = nullptr;
    return *this;
}

bool Object::isValueT() const { return Type::isValueT(_typeId); }
bool Object::isRValue() const {
    return isValueT() || !_refCount || (*_refCount <= 1);
}

Object Object::copy() const {
    if (!_refCount || Type::isValueT(_typeId)) {
        return copyValue();
    }
    Object obj;
    obj.value.ptr = _func(TypeFuncAction::COPY, value.ptr);
    obj._func = _func;
    obj._typeId = _typeId;
    obj._refCount = new uint32_t(1);
    return obj;
}

void Object::release() {
    //    std::println("release");
    if (!_refCount || Type::isValueT(_typeId))
        return;
    if (--(*_refCount) == 0) {
        _func(TypeFuncAction::DEALLOCATE, value.ptr);
        _func = Nil::nxTypeFunc;
        value.ptr = nullptr;
        delete _refCount;
        _refCount = nullptr;
    }
}
Object Object::copyValue() const {
    Object obj;
    obj.value = value;
    obj._func = _func;
    obj._typeId = _typeId;
    return obj;
}
} // namespace nx
