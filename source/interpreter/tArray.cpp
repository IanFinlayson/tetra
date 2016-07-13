/*
 * This class wraps an array of tData<void*> used to make arrays and
 * multi-dimensional arrays
 */

#include "backend.h"

using std::string;
using std::vector;

// Code for when elements was a pointer
TArray::TArray() {
  // default constructs elements and initialize vector pointer
}

// Because default behavior of arrays in tetra is aliasing, the copy constructor
// and copy assignment will also alias rather than deep copy
TArray::TArray(const TArray& other) : elements(other.elements) {}

TArray& TArray::operator=(const TArray& other) {
  if (this != &other) {
    elements = other.elements;
  }
  return *this;
}

// Stubbed The only thing that needs to be destructed is the pointerm which is
// destructed by default
TArray::~TArray() {}

// Returns a reference to a TData containing a pointer to the data associated
// with index. Note that this can be used for reading AND writing
TData<void*>& TArray::elementAt(tint index) {
  // check for out-of-bounds access (note that parameter is unsigned, cannot be
  // < 0
  if (index < elements->size()) {
    return (*elements)[(unsigned long int) index];
  } else {
    // Print error message and terminate
    tstringstream message;
    message << "Attempted to access an array out of bounds.\nRequested element "
            << index << " for array of size " << elements->size();
    Error e(message.str(), 0);
    throw e;
  }
}

// The const version of elementAt
const TData<void*>& TArray::elementAt(tint index) const {
  // check for out-of-bounds access (note that parameter is unsigned, cannot be
  // < 0
  if (index < elements->size()) {
    return (*elements)[(unsigned long int)index];
  } else {
    std::basic_stringstream<char,char_traits<char>, gc_allocator<char> > message;
    message << "Attempted to access an array out of bounds.\nRequested element "
            << index << " for array of size " << elements->size();
    Error e(message.str(), 0);
    throw e;
  }
}

// Adds the giveen element to the end of the array. Note that is it is pointing
// at allocated memory, it should have had its setDeletableType method called
void TArray::addElement(const TData<void*>& pElement) {
  elements->push_back(pElement);
}

// Get the start and end iterators to the underlying vector container
const std::vector<
    TData<void*>, gc_allocator<TData<void*> > /* ,mmap_allocator<TData<void*> >*/>::const_iterator
TArray::begin() const {
  return elements->begin();
}

const std::vector<
    TData<void*>, gc_allocator<TData<void*> >/*, mmap_allocator<TData<void*> >*/>::const_iterator
TArray::end() const {
  return elements->end();
}

tint TArray::size() const { return elements->size(); }

// Friend function, operator for printing
std::ostream& operator<<(std::ostream& outStream, const TArray obj) {
  tint size = obj.size();
  outStream << "[";
  // Print all elements except the last one
  for (unsigned long x = 0; x < (unsigned long int) size; x++) {
    const TData<void*>& element = obj.elementAt(x);
    // as usual, we must typecheck the returned elemntes so we can print them
    // properly
    switch (element.getPointedTo().getKind()) {
      case TYPE_INT:
        outStream << *static_cast<tint*>(element.getData());
        break;
      case TYPE_REAL:
        outStream << *static_cast<double*>(element.getData());
        break;
      case TYPE_BOOL:
        outStream << *static_cast<tbool*>(element.getData());
        break;
      case TYPE_STRING:
        outStream << *static_cast<string*>(element.getData());
        break;
      case TYPE_VECTOR:
        outStream << *static_cast<TArray*>(element.getData());
        break;
      default:
        outStream << "??";
    }

    // If this was not the last element, print a comma
    if (x + 1 != (unsigned long int) size) {
      outStream << ", ";
    }
  }
  // close bracket
  outStream << "]";
  return outStream;
}
