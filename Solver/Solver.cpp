/**
* @author Tim Luchterhand
* @date 27.11.24
* @brief
*/

#include "Solver.hpp"
#include "util/exception.hpp"

namespace sat {
    Solver::Solver(unsigned numVariables) {
        this->numVariables = numVariables;
        model.resize(numVariables, TruthValue::Undefined);
    }

    bool Solver::addClause(Clause clause) {
        if (clause.empty()) {
            return false;
        }

        if (clause.size() == 1) {
            Literal unitLiteral = clause[0];
            if (!assign(unitLiteral)) {
                return false; // La clause unitaire viole le modèle actuel
            }
        }
        clauses.push_back(std::make_shared<Clause>(std::move(clause)));
        return true;
    }

    auto Solver::rebase() const -> std::vector<Clause> {
        std::vector<Clause> reducedClauses;

        for (const auto& clausePtr : clauses) {
            Clause reducedClause;

            for (const auto& literal : *clausePtr) {
                if (satisfied(literal)) {
                    reducedClause.clear();
                    reducedClause.push_back(literal);
                    break;
                }
                else if (!falsified(literal)) {
                    reducedClause.push_back(literal);
                }
            }
            if (!reducedClause.empty()) {
                reducedClauses.push_back(std::move(reducedClause));
            }
        }
        return reducedClauses;
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
        TruthValue value = val(var);
        unsigned varIndex = var.get();

        if (value != TruthValue::Undefined){
            return satisfied(l);
        }

        model[varIndex] = l.sign() == 1 ? TruthValue::True : TruthValue::False;
        return true;
    }

    bool Solver::unitPropagate() {
        std::vector<Clause> unitClauses;
        std::vector<Clause> newClauses;

        for (const auto& clausePtr : clauses) {
            if (clausePtr->size()==1) {
                unitClauses.push_back(std::move(*clausePtr));
            }
        }

        while (!unitClauses.empty()) {
            Clause unitClause = unitClauses.back();
            unitClauses.pop_back();
            Literal l = unitClause[0];

            if (!assign(l)) {
                return false;
            }

            newClauses = rebase();
            clauses.clear();
            for (const auto& clause : newClauses) {
                addClause(clause);
            }

            for (const auto& clausePtr : clauses) {
                if (clausePtr->size()==1) {
                    unitClauses.push_back(std::move(*clausePtr));
                }
            }     
        }
        return true;
    }
} // sat