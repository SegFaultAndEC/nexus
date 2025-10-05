#ifndef NEXUS_DEF_HPP
#define NEXUS_DEF_HPP

#include "error/error.hpp"
#include "evaluator/evaluator.hpp"
#include "lexer/lexer.hpp"
#include "parser/parser.hpp"

#define NX_REGISTER_FUNC_NAME "_nexusRegisterFunc_"

#if defined(_WIN32) || defined(_WIN64)
#define NX_REGISTER_BEGIN                                                      \
    extern "C" {                                                               \
    __declspec(dllexport) void _nexusRegisterFunc_(::nx::State &state) {
#else
#define NX_REGISTER_BEGIN                                                      \
    extern "C" {                                                               \
    void _nexusRegisterFunc_(::nx::State &state) {
#endif

#define NX_REGISTER_END                                                        \
    }                                                                          \
    }

namespace nx {
using RegisterFunc = void (*)(::nx::State &state);
}

#endif // NEXUS_DEF_HPP
