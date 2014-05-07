#include <string>
#include <cstdlib>

#include "tetra.hpp"

void fail(const std::string& mesg, int lineno) {
  cerr << "Tetra error: " << mesg;
  if (lineno) {
    cout << " (Line " << lineno << ")";
  }
  cerr << endl;
}



#endif



