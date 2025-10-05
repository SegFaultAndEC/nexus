#ifndef NEXUS_ERROR_HPP
#define NEXUS_ERROR_HPP

#include <print>

#define NX_ERROR(...) throw ::nx::NxError(::std::format(__VA_ARGS__));

namespace nx {
class NxError : public std::exception {
  public:
    explicit NxError(std::string &&msg) : _msg(std::move(msg)) {}
    const char *what() const noexcept override { return _msg.c_str(); }
    void show(int line = -1) const {
        if (line != -1)
            std::println("\033[31m[line{}]{}\033[0m", line, _msg);
        else
            std::println("\033[31m{}\033[0m", _msg);
    }

  private:
    std::string _msg{};
};
} // namespace nx

#endif // NEXUS_ERROR_HPP
