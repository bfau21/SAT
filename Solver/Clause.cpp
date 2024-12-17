/**
* @author Tim Luchterhand
* @date 26.11.24
* @brief
*/

#include <cassert>
#include <algorithm>

#include "Clause.hpp"
#include "util/exception.hpp"

namespace sat {
    //TODO implementation here

    Clause::Clause(std::vector<Literal> literals) {
        throw NOT_IMPLEMENTED;
    }

    short Clause::getRank(Literal l) const {
        throw NOT_IMPLEMENTED;
    }

    std::size_t Clause::getIndex(short rank) const {
        throw NOT_IMPLEMENTED;
    }

    bool Clause::setWatcher(Literal l, short watcherNo) {
        throw NOT_IMPLEMENTED;
    }

    auto Clause::begin() const -> std::vector<Literal>::const_iterator {
        throw NOT_IMPLEMENTED;
    }

    auto Clause::end() const -> std::vector<Literal>::const_iterator {
        throw NOT_IMPLEMENTED;
    }

    bool Clause::isEmpty() const {
        throw NOT_IMPLEMENTED;
    }

    Literal Clause::operator[](std::size_t index) const {
        throw NOT_IMPLEMENTED;
    }

    std::size_t Clause::size() const {
        throw NOT_IMPLEMENTED;
    }

    Literal Clause::getWatcherByRank(short rank) const {
        throw NOT_IMPLEMENTED;
    }

    bool Clause::sameLiterals(const Clause &other) const {
        throw NOT_IMPLEMENTED;
    }

}
