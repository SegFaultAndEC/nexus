#ifndef NEXUS_LEXER_HPP
#define NEXUS_LEXER_HPP

#include "token.hpp"
#include <string_view>
#include <vector>

namespace nx {
std::vector<Token> lexer(std::string_view line);
}

#endif // NEXUS_LEXER_HPP
