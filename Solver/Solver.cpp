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
        std::vector<Clause> unitClauses; // Pour stocker les clauses unitaires sans doublons

        for (const auto& clausePtr : clauses) {
            std::vector<Literal> satisfyingLiterals;
            Clause reducedClause;

            for (const auto& literal : *clausePtr) {
                if (satisfied(literal)) {
                    satisfyingLiterals.push_back(literal);
                } else if (!falsified(literal)) {
                    reducedClause.push_back(literal);
                }
            }

            if (!satisfyingLiterals.empty()) {
                // Ajouter des clauses unitaires pour chaque littéral satisfaisant en évitant les doublons
                for (const auto& lit : satisfyingLiterals) {
                    Clause unitClause({lit});
                    if (std::find(unitClauses.begin(), unitClauses.end(), unitClause) == unitClauses.end()) {
                        unitClauses.push_back(unitClause);
                    }
                }
            } else if (!reducedClause.empty()) {
                // Ajouter la clause réduite si elle n'est pas vide
                reducedClauses.push_back(std::move(reducedClause));
            }
        }

        // Ajouter les clauses unitaires uniques à la fin
        reducedClauses.insert(reducedClauses.end(), unitClauses.begin(), unitClauses.end());

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
        bool changed = true;
        while (changed) {
            changed = false;
            for (const auto& clausePtr : clauses) {
                int undefinedCount = 0;
                Literal unitLiteral = 0;
                bool clauseSatisfied = false;
                for (const Literal& lit : *clausePtr) {
                    if (satisfied(lit)) {
                        clauseSatisfied = true; // Clause satisfaite, passe à la suivante
                        break;
                    }
                    if (!falsified(lit)) {
                        ++undefinedCount;
                        unitLiteral = lit;
                    }
                }
                if (clauseSatisfied) {
                    continue; // Clause déjà satisfaite, pas besoin de la traiter
                }
                if (undefinedCount == 0) {
                    // Tous les littéraux sont falsifiés : conflit détecté
                    return false;
                }
                if (undefinedCount == 1) {
                    // Clause unitaire détectée, assignation nécessaire
                    if (!assign(unitLiteral)) {
                        return false; // Propagation échoue
                    }
                    changed = true;
                }
            }
        }
        return true;
    }
} // sat