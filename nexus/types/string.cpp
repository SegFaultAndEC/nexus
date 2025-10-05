#include "string.hpp"
#include <array>
namespace nx {
String::String() = default;

String::String(const String &string) : _str(string._str) {}

String::String(String &&string) noexcept : _str(std::move(string._str)) {
    string._str.clear();
}

String::String(std::string_view sv) { _str = std::move(std::string(sv)); }

String &String::operator=(const String &string) {
    if (this != &string) {
        _str = string._str;
    }
    return *this;
}

String &String::operator=(String &&string) noexcept {
    if (this != &string) {
        _str = std::move(string._str);
        string._str.clear();
    }
    return *this;
}

void *String::nxTypeFunc(TypeFuncAction action, void *ptr) {
    switch (action) {
    case TypeFuncAction::ALLOCATE:
        return new String();
    case TypeFuncAction::DEALLOCATE: {
        delete reinterpret_cast<String *>(ptr);
        break;
    }
    case TypeFuncAction::COPY:
        return new String(*reinterpret_cast<String *>(ptr));
    }
    return nullptr;
}

std::string String::toStdString() const noexcept { return _str; }

std::array<char, 5> String::char32ToChar8(char32_t ch) {
    std::array<char, 5> result{};
    if (ch <= 0x7F) {
        result[0] = static_cast<char>(ch);
        result[1] = '\0';
    } else if (ch <= 0x7FF) {
        result[0] = static_cast<char>(0xC0 | (ch >> 6));
        result[1] = static_cast<char>(0x80 | (ch & 0x3F));
        result[2] = '\0';
    } else if (ch <= 0xFFFF && !(ch >= 0xD800 && ch <= 0xDFFF)) {
        result[0] = static_cast<char>(0xE0 | (ch >> 12));
        result[1] = static_cast<char>(0x80 | ((ch >> 6) & 0x3F));
        result[2] = static_cast<char>(0x80 | (ch & 0x3F));
        result[3] = '\0';
    } else if (ch <= 0x10FFFF) {
        result[0] = static_cast<char>(0xF0 | (ch >> 18));
        result[1] = static_cast<char>(0x80 | ((ch >> 12) & 0x3F));
        result[2] = static_cast<char>(0x80 | ((ch >> 6) & 0x3F));
        result[3] = static_cast<char>(0x80 | (ch & 0x3F));
        result[4] = '\0';
    } else {
        result[0] = '?';
        result[1] = '\0';
    }
    return result;
}

void String::append(const String &string) { _str.append(string._str); }
void String::append(const char32_t character) {
    _str.append(char32ToChar8(character).data());
}
char32_t String::at(int64_t index) {
    size_t indexs = static_cast<size_t>(index);
    if (indexs >= size()) {
        NX_ERROR("Error0x0010:Index out of range.");
    }
    return static_cast<char32_t>(_str[indexs]);
}
int64_t String::size() { return static_cast<int64_t>(_str.size()); }
} // namespace nx
