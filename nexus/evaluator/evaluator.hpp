#ifndef NEXUS_EVALUATOR_HPP
#define NEXUS_EVALUATOR_HPP

#include "parser/node.hpp"
#include "state/state.hpp"
#include "types/object.hpp"

namespace nx {

class Evaluator {
  public:
    explicit Evaluator(State &state);
    Object eval(const ASTNode *node);
    Object evalAssign(const ASTNode *node);
    Object evalConstant(const Token &token);
    Object evalList(const ASTNode *node);
    Object evalVar(const Token &token);
    Object evalPrefix(const ASTNode *node);
    Object evalBinaryOp(const ASTNode *node);
    Object evalFunction(const ASTNode *node);
    Object evalIndex(const ASTNode *node);
    Object evalFlow(const ASTNode *node);

  private:
    State &_state;
};
} // namespace nx

#endif // NEXUS_EVALUATOR_HPP
