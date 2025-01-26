/**
* @author Tim Luchterhand
* @date 26.11.24
* @file Clause.hpp
* @brief Contains the class Clause that consists of one or more literals
*/

#ifndef CLAUSE_HPP
#define CLAUSE_HPP

#include <vector>
#include <ostream>

#include "util/concepts.hpp"
#include "basic_structures.hpp"

namespace sat {

    /**
     * Concept modeling a clause. A clause is some range of literals, i.e. it must have accessible members
     * begin() and end() that return an iterator to the first or pas the end elements respectively. For example
     * std::vector<Literal> satisfies this concept.
     */
    template<typename T>
    concept clause_like = concepts::typed_range<T, Literal>;

    /*
     * Your implementation of a clause here. If you want to implement unit propagation using counters, then a simple
     * std::vector<Literal> will do the job. For example like this:
     *
     * using Clause = std::vector<Literal>;
     * No more implementation to be done her. The rest is handled in the solver.
     *
     * If you want to do unit propagation using watch-literals, then you can use the template below.
     */
    using Clause = std::vector<Literal>;
}


#endif //CLAUSE_HPP
