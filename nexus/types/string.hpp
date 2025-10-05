#ifndef NEXUS_STRING_HPP
#define NEXUS_STRING_HPP
#include "error/error.hpp"
#include "type.hpp"
#include <string>
namespace nx {
class String {
    NX_OBJECT(String)
  public:
    String();
    String(const String &string);
    String(String &&string) noexcept;
    explicit String(std::string_view sv);
    String &operator=(const String &string);
    String &operator=(String &&string) noexcept;
    static void *nxTypeFunc(TypeFuncAction action, void *ptr = nullptr);
    std::string toStdString() const noexcept;

    static std::array<char, 5> char32ToChar8(char32_t ch);

    void append(const String &string);
    void append(const char32_t character);
    int64_t size();
    char32_t at(int64_t index);


  private:
    std::string _str{};
};
} // namespace nx

#endif // NEXUS_STRING_HPP
