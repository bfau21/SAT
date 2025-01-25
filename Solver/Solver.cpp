/**
* @author Tim Luchterhand
* @date 27.11.24
* @brief
*/

#include "Solver.hpp"
#include "util/exception.hpp"
#include <iostream>

namespace sat {
    Solver::Solver(unsigned numVariables) : numVariables(numVariables) {
        model.resize(numVariables, TruthValue::Undefined);
        assigned.resize(numVariables, false);
    }

    bool Solver::addClause(Clause clause) {
        if (clause.empty()) {
            return false; // Une clause vide est toujours fausse
        }

        // Vérifie si la clause est unitaire
        if (clause.size() == 1) {
            Literal unitLiteral = clause[0];
            if (!assign(unitLiteral)) {
                return false; // La clause unitaire viole le modèle actuel
            }
        }

        // Ajoute la clause dans le vecteur des clauses
        clauses.push_back(std::make_shared<Clause>(std::move(clause)));
        return true;
    }

    auto Solver::rebase() const -> std::vector<Clause> {
        std::vector<Clause> reducedClauses;

        for (const auto& clausePtr : clauses) {
            Clause reducedClause;

            // Liste pour les littéraux satisfaisants
            std::vector<Literal> satisfiedLiterals;

            // Parcours des littéraux dans la clause
            for (const auto& literal : *clausePtr) {
                if (satisfied(literal)) {
                    satisfiedLiterals.push_back(literal); // Ajoute les littéraux satisfaisants
                } else if (!falsified(literal)) {
                    reducedClause.push_back(literal); // Ajoute les littéraux non falsifiés
                }
            }

            // Si des littéraux satisfaisants ont été trouvés, on garde seulement ces littéraux
            if (!satisfiedLiterals.empty()) {
                reducedClause = satisfiedLiterals; // Remplace la clause par les littéraux satisfaisants
            }

            // Ajouter la clause réduite à la liste si elle n'est pas vide
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