#ifndef JS_CMP_LEXER_OPTIMIZATION_HPP
#define JS_CMP_LEXER_OPTIMIZATION_HPP
#include "../AST/Node.hpp"


namespace Lexer::Opt {

typedef enum : std::uint8_t {
    OPTIMIZE_TYPE_DEDUCTION = 0x1 /**< Flag for type deduction optimization. */
} OptimizationFlag;

using flag_t = uint32_t;


class Optimizer {
public:
    explicit Optimizer(std::vector<AST::Stmt::Ptr>& ast);

    void optimize(flag_t flags);
    std::string transpile() const;
private:
    std::vector<AST::Stmt::Ptr>& ast;
};
}
#endif //JS_CMP_LEXER_OPTIMIZATION_HPP