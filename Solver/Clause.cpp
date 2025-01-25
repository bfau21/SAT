/**
* @author Tim Luchterhand
* @date 26.11.24
* @brief
*/

#include <cassert>
#include <algorithm>
#include <ostream>

#include "Clause.hpp"
#include "util/exception.hpp"

namespace sat {
   std::ostream& operator<<(std::ostream& os, const Clause& clause) {
      os << "[";
      for (size_t i = 0; i < clause.size(); ++i) {
         os << clause[i]; // Utilise operator<< de Literal
         if (i != clause.size() - 1) {
            os << " ∨ ";
         }
      }
      os << "]";
      return os;
   }
}
