/**
* @author Tim Luchterhand
* @date 27.11.24
* @brief
*/

#include "Solver.hpp"
#include "util/exception.hpp"

namespace sat {
    Solver::Solver(unsigned numVariables) {
        throw NOT_IMPLEMENTED;
    }

    bool Solver::addClause(Clause clause) {
        throw NOT_IMPLEMENTED;
    }

    auto Solver::rebase() const -> std::vector<Clause> {
        throw NOT_IMPLEMENTED;
    }

    TruthValue Solver::val(Variable x) const {
        throw NOT_IMPLEMENTED;
    }

    bool Solver::satisfied(Literal l) const {
        throw NOT_IMPLEMENTED;
    }

    bool Solver::falsified(Literal l) const {
        throw NOT_IMPLEMENTED;
    }

    bool Solver::assign(Literal l) {
        throw NOT_IMPLEMENTED;
    }

    bool Solver::unitPropagate() {
        throw NOT_IMPLEMENTED;
    }
} // sat