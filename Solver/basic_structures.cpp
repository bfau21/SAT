/**
* @author Tim Luchterhand
* @date 26.11.24
* @brief
*/

#include "basic_structures.hpp"
#include "util/exception.hpp"

namespace sat {


    Variable::Variable(unsigned val) {
        this->value = val;
    }

    unsigned Variable::get() const {
        return this->value;
    }

    bool Variable::operator==(Variable other) const {
        return this->value == other.value;
    }

    Literal::Literal(unsigned val) {
        this->lit = val;
    }

    unsigned Literal::get() const {
        return this->lit;
    }

    Literal Literal::negate() const {
        return Literal(this->lit + 1);
    }

    short Literal::sign() const {
        if (this->lit % 2 != 0) {
            return 1;
        }
        return -1;
    }

    bool Literal::operator==(Literal other) const {
        if (this->lit == other.lit && this->sign() == other.sign()) {
            return true;
        }
        return false;
    }

    Literal pos(Variable x) {
        return Literal(x.get()*2+1);
    }

    Literal neg(Variable x) {
        return Literal(x.get()*2);
    }

    Variable var(Literal l) {
        return Variable(l.get()/2);
    }
}
