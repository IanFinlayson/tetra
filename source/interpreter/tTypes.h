/* This file defines basic tetra data types as
 * wrappers around their c++ counterparts */



class tbool {
  public:
    /* constructors */
    tbool() {this->b = false;} 
    tbool(const bool& b) {this->b = b;} 
    tbool(const tbool& other) {this->b = other.b;}

    /*assignment */
    tbool operator=(const tbool& other) {
      this->b = other.b;
      return *this;
    }

    /* conversion */
    operator bool const (){return this->b;}

    /* logical operators */
    tbool operator&&(const tbool& other) {return tbool(this->b && other.b);} 
    tbool operator||(const tbool& other) {return tbool(this->b || other.b);} 
    tbool operator!() {return tbool(!this->b);}

    /* comparison operators */ 
    tbool operator==(const tbool& other) {return tbool(this->b == other.b);}
    tbool operator<(const tbool& other) {return tbool(this->b < other.b);}
    tbool operator<=(const tbool& other) {return tbool(this->b <= other.b);}
    tbool operator>(const tbool& other) {return tbool(this->b > other.b);}
    tbool operator>=(const tbool& other) {return tbool(this->b >= other.b);}

    /* tbool output */
    friend ostream& operator<<(ostream& os, const tbool& tb) {
      tb.b ? os << "true" : os << "false";
      return os;
    }

  private:
    bool b;
};


class tint {
  public:
  private:
};

class treal {
  public:
  private:
};
/*
class tstring {
  public:
  private:
};

*/
class tnone {
  public:
  private:
};
