/**
* @author Tim Luchterhand
* @date 27.11.24
* @brief
*/

#include "Solver.hpp"
#include "util/exception.hpp"

namespace sat {
    Solver::Solver(unsigned numVariables) : numVariables(numVariables) {
        model.resize(numVariables, TruthValue::Undefined);
        assigned.resize(numVariables, false);
    }

    bool Solver::addClause(Clause clause) {
        if (clause.empty()) {
            return false;
        }

        ClausePointer clausePointer = std::make_shared<Clause>(std::move(clause));

        clauses.push_back(std::make_shared<Clause>(clause));
        return true;
    }

    auto Solver::rebase() const -> std::vector<Clause> {
        throw NOT_IMPLEMENTED;
    }

    TruthValue Solver::val(Variable x) const {
        unsigned varIndex = x.get();
        if (varIndex >= numVariables) {
            throw std::out_of_range("Variable index out of range");
        }
        return model[varIndex];
    }

    bool Solver::satisfied(Literal l) const {
        Variable var = sat::var(l);
        TruthValue value = val(var);
        return (l.sign() == 1 && value == TruthValue::True) || 
                (l.sign() == -1 && value == TruthValue::False);
    }

    bool Solver::falsified(Literal l) const {
        Variable var = sat::var(l);
        TruthValue value = val(var);
        return (l.sign() == 1 && value == TruthValue::False) || 
                (l.sign() == -1 && value == TruthValue::True);
    }

    bool Solver::assign(Literal l) {
        Variable var = sat::var(l);
        unsigned varIndex = var.get();

        if (assigned[varIndex]) {
            return satisfied(l);
        }

        model[varIndex] = l.sign() == 1 ? TruthValue::True : TruthValue::False;
        assigned[varIndex] = true;

        //backtracking
        trail.push_back(l);
        return true;
    }

    bool Solver::unitPropagate() {
        throw NOT_IMPLEMENTED;
    }
} // sat