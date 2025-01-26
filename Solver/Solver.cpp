/**
* @author Tim Luchterhand
* @date 27.11.24
* @brief
*/

#include "Solver.hpp"
#include "util/exception.hpp"
#include <iostream>
#include <queue>
#include <set>

namespace sat {
    Solver::Solver(unsigned numVariables) : numVariables(numVariables), counters(numVariables, 0) {
        model.resize(numVariables, TruthValue::Undefined);
        assigned.resize(numVariables, false);
        unitLiterals.clear();
    }

    bool Solver::addClause(Clause clause) {
        if (clause.empty()) {
            return false;
        }

        // Mise à jour des compteurs pour les variables dans la clause
        for (const auto& literal : clause) {
            counters[sat::var(literal).get()]++;
        }

        // Vérifie si la clause est unitaire
        if (clause.size() == 1) {
            Literal unitLiteral = clause[0];

            // Vérifie si la clause unitaire existe déjà dans les clauses
            for (const auto& existingClause : clauses) {
                if (existingClause->size() == 1 && existingClause->front() == unitLiteral) {
                    // Si la clause unitaire existe déjà, ne pas l'ajouter
                    return true; // La clause est ignorée, donc on considère que l'ajout est réussi
                }
            }

            // Si la clause unitaire n'existe pas, on l'assigne
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
            std::set<Literal> uniqueSatisfiedLiterals; // Utilisé pour éviter les doublons

            for (const auto& literal : *clausePtr) {
                if (satisfied(literal)) {
                    uniqueSatisfiedLiterals.insert(literal); // Ajoute le littéral satisfaisant (sans doublon)
                } else if (!falsified(literal)) {
                    reducedClause.push_back(literal); // Ajoute les littéraux non falsifiés
                }
            }

            // Crée une clause unitaire pour chaque littéral satisfaisant
            for (const auto& satisfiedLiteral : uniqueSatisfiedLiterals) {
                reducedClauses.push_back({satisfiedLiteral}); // Clause unitaire
            }

            // Si aucun littéral satisfaisant n'a été trouvé, ajoute la clause réduite si elle n'est pas vide
            if (uniqueSatisfiedLiterals.empty() && !reducedClause.empty()) {
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
        unitLiterals.push_back(l);

        //backtracking
        trail.push_back(l);
        return true;
    }

    bool Solver::unitPropagate() {
        bool resultat = true;

    while (!unitLiterals.empty()) {
        Literal unit = unitLiterals.back();
        unitLiterals.pop_back();

        if (!assign(unit)) {
            return false;
        }

        std::vector<Clause> new_clauses = rebase();

        for (const auto& clause : new_clauses) {
            resultat = resultat && addClause(clause);
            if (clause.size() == 1) {
                Literal lit = clause[0];
                if (val(var(lit)) == TruthValue::Undefined) {
                    unitLiterals.push_back(lit);
                }
            }
        }

    }
    return resultat;
    }
} // sat