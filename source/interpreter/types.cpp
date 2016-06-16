/* functions for type handling and inference */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include "frontend.h"
#include "parser.genh"
#include "backend.h"

extern Node* root;

/* the symbol table for storing constants and globals */
map<std::string, Symbol> globals;

/* map for storing classes */
map<std::string, ClassContext> classes;

/* map for storing free functions */
FunctionMap functions;

/* prototypes */
void inferBlock(Node*, Node* );
void checkClassTypes(Node*);
DataType* inferExpression(Node*,Node*); 

/* return a string of an int */
string itoa(int num) {
  stringstream ss;
  ss << num;
  return ss.str();
}

/* return a string of a data type */
string typeToString(DataType* t) {
  switch (t->getKind()) {
    case TYPE_NONE:
      return "none";
    case TYPE_INT:
      return "int";
    case TYPE_REAL:
      return "real";
    case TYPE_STRING:
      return "string";
    case TYPE_BOOL:
      return "bool";
    case TYPE_MUTEX:
      return "mutex";
    case TYPE_TASK:
      return "task";
    case TYPE_VECTOR:
      return "[" + typeToString(&((*(t->subtypes))[0])) + "]";
    case TYPE_DICT:
      return "{" + typeToString(&((*(t->subtypes))[0])) + ":" 
        + typeToString(&((*(t->subtypes))[1])) + "}";
    case TYPE_TUPLE:{
                      std::string typeString = "(";
                      for(unsigned long int i = 0; i < t->subtypes->size(); i++){
                        typeString += typeToString(&((*(t->subtypes))[i])) + ","; 
                      }
                      /* if the tuple has more than one element ... */
                      if(t->subtypes->size() > 1)
                        /* then get rid of the trailing comma */
                        typeString[typeString.size()-1] = '\0';

                      return typeString + ")";
                    }
    case TYPE_CLASS:
                    return *(t->className);
    case TYPE_FUNCTION:{
                         return typeToString(&((*(t->subtypes))[0])) + "->" 
                           + typeToString(&((*(t->subtypes))[1]));
                       }
    default:
                       throw Error("typeToString: Unknown data type");
  }
}

/* class context functions */
ClassContext::ClassContext(std::string name) {
  this->name = name;
}

ClassContext::ClassContext() {
  this->name = "#ImBroken";
}

string ClassContext::getName(){ 
  return this->name;
}

bool ClassContext::hasMember(std::string varName) {
  return this->members.count(varName); 
}

bool ClassContext::hasMethod(DataType* type, std::string name) {
  return this->methods.hasFunction(type,name);
}

void ClassContext::addMember(Symbol sym) {
  members[sym.getName()] = sym; 
}

void ClassContext::addMethods(Node* root){
  this->methods.build(root);
}

/* Given a class node, this function adds the member vars 
 * into the class map (Cannot type check yet as this class 
 * may have members that are dependent on
 * classes that have not been seen yet.)*/
void ClassContext::addMembers(Node* node) {
  /* add this part */
  if (node->kind() == NODE_IDENTIFIER) {
    this->addMember(Symbol(node->getString(), node->type(), 
          node->getLine())); 
  } else if (node->kind() == NODE_CLASS_PART) {
    /* recursively add other parts */
    this->addMembers(node->child(0));
    if(node->child(1)){
      this->addMembers(node->child(1));
    }
  }
}

Symbol ClassContext::getMember(std::string name){
  return members[name]; 
}

const Node* ClassContext::getMethod(DataType* type, std::string name) {
  return methods.getFunctionNode(type, name);
}

DataType* ClassContext::getMethods(std::string name) {
  return methods.getFunctionsNamed(name);
}

bool ClassContext::hasMethodNamed(std::string name) {
  return methods.hasFuncNamed(name);
}

std::map<std::string,Node*> ClassContext::removeInits(){

  /* remove any inits from the list of methods 
   * and return them */
  return methods.remove("init");
}

/* data type functions */
DataType::DataType(DataTypeKind kind) {
  this->kind = kind;
  this->subtypes = new std::vector<DataType>;
  this->className = new std::string;
}

DataType::DataType(const DataType& other) {
  this->kind = other.kind;  
  this->subtypes = new std::vector<DataType>;
  this->className = new std::string;
  *this->subtypes = *other.subtypes;
  *this->className = *other.className;
}

DataType::~DataType() {
  delete(className);
  delete(subtypes);
}

/* return true if the type represents an empty
 * container (i.e. is a dict/vector with no
 * subtype. */
bool DataType::isEmptyContainerType() const {
  return ((this->getKind() == TYPE_VECTOR 
        || this->getKind() == TYPE_DICT)
      && this->subtypes->size() == 0);
}


DataTypeKind DataType::getKind() const { return kind; }

/* compare two data types for equality */
bool operator==(const DataType& lhs, const DataType& rhs) {
  /* if they're not the same kind, fail */
  if (lhs.getKind() != rhs.getKind()) {
    return false;
  } 

  /* if they're vectors or dicts and either is empty.. */
  if (lhs.getKind() == TYPE_VECTOR 
      && (lhs.isEmptyContainerType() || rhs.isEmptyContainerType())) {
    /* then they match! */
    return true;
  }

  if (lhs.getKind() == TYPE_VECTOR) {
    /* recursively ensure the subtypes match */
    return (*(lhs.subtypes))[0] == (*(rhs.subtypes))[0];
  }

  /* for dictionaries, recursively check the key and value types */
  if (lhs.getKind() == TYPE_DICT) {
    return (((*(lhs.subtypes))[0] == (*(rhs.subtypes))[0])
      && ((*(lhs.subtypes))[1] == (*(rhs.subtypes))[1]));
  }

  /* for classes, just check the names */
  if (lhs.getKind() == TYPE_CLASS) {
    return lhs.className == rhs.className;
  }

  /* for tuples and functions, check that they have the same
   * number of subtypes */
  if ((lhs.subtypes->size() && rhs.subtypes->size()) 
      && lhs.subtypes->size() != rhs.subtypes->size()){
    return false;
  }

  /* if they are both tuple or both functions and we made it 
   * here, then they have the same number of subtypes, 
   * so we need to compare the subtypes recursively. */
  if (lhs.getKind() == TYPE_TUPLE 
      || lhs.getKind() == TYPE_FUNCTION) {
    bool same = true;
    unsigned long int i = 0;
    while (same && i < lhs.subtypes->size()) {
      same = ((*(lhs.subtypes))[i] == (*(rhs.subtypes))[i]);
      i++;
    }
    return same;
  }

  /* otherwise return true */
  return true;
}

/* compare two data types for in-equality */
bool operator!=(const DataType& lhs, const DataType& rhs) {
  return !(lhs == rhs);
}

DataType DataType::operator=(const DataType& other) {
  kind = other.kind;  
  *subtypes = *other.subtypes;
  *className = *other.className;
  return *this;
}

/* infer the param subtype from a function call */
void buildParamTupleType(DataType* type, Node* node, Node* func) {
  if (node && node->kind() == NODE_ACTUAL_PARAM_LIST) {
    buildParamTupleType(type, node->child(0), func); 
    buildParamTupleType(type, node->child(1), func); 
  } else if (node) {
    type->subtypes->push_back(*inferExpression(node,func));
  } else {
    return;
  } 
}

/* add the parameters of a function into its symtable */
void addParams(Node* params, Node* func) {
  if (!params) return;

  /* if it's more than one, recurse on both */
  if (params->kind() == NODE_FORMAL_PARAM_LIST) {
    addParams(params->child(0), func);
    addParams(params->child(1), func);
  }

  /* else if it's just one param, handle it */
  else if (params->kind() == NODE_DECLARATION) {
    func->insertSymbol(
        Symbol(params->getString(), params->type(), params->getLine()));
  }
}


/* infer the types of a print call */
DataType* inferPrint(Node* pcall, Node* func) {
  /* just infer each expression, but we don't care what it is */

  /* get the first arg */
  Node* arg = pcall->child(1);

  /* while there are arguments */
  while (arg) {
    /* if this arg is a param, check the child */
    if ((arg->kind() == NODE_ACTUAL_PARAM_LIST) && (arg->numChildren() >= 1)) {
      inferExpression(arg->child(0), func);
    }
    /* else if it's some other child, check it itself */
    else {
      inferExpression(arg, func);
    }

    /* if the next arg is another arg, move on */
    if ((arg->numChildren() >= 2) &&
        (arg->child(1)->kind() == NODE_ACTUAL_PARAM_LIST)) {
      arg = arg->child(1);
    }

    /* else just check it and we're done */
    else if (arg->numChildren() >= 2) {
      inferExpression(arg->child(1), func);
      arg = NULL;
    } else {
      arg = NULL;
    }
  }

  return NULL;
}

DataType* inferLen(Node* funcall, Node* func) {
  /* check that there is one argument */
  if (funcall->numChildren() != 2 || funcall->child(1)->numChildren() != 1) {
    throw Error("len function expects one argument", funcall->getLine());
  }

  /* infer the argument and capture its type */
  DataType* t = inferExpression(funcall->child(1)->child(0), func);

  /* check that it is a vector or a string */
  if ((t->getKind() != TYPE_VECTOR) && (t->getKind() != TYPE_STRING)
      && (t->getKind() != TYPE_TUPLE) && (t->getKind() != TYPE_DICT)) {
    throw Error("len function must be called on string, vector, tuple, or dictionary",
        funcall->getLine());
  }

  /* should return an int */
  return new DataType(TYPE_INT);
}

DataType* inferRead(Node* funcall) {
  /* make sure there are no parameters */
  if (funcall->numChildren() > 1) {
    throw Error(funcall->getString() + " should not have any parameters",
        funcall->getLine());
  }

  /* get the return type right */
  if (funcall->child(0)->getString() == "read_string") return new DataType(TYPE_STRING);
  if (funcall->child(0)->getString() == "read_int") return new DataType(TYPE_INT);
  if (funcall->child(0)->getString() == "read_real") return new DataType(TYPE_REAL);
  if (funcall->child(0)->getString() == "read_bool") return new DataType(TYPE_BOOL);

  throw Error("This should not happen!", funcall->getLine());
}

/* this function checks if a function call is part of the standard library and
 * infers it */
DataType* inferStdlib(Node* funcall, Node* func, bool& is_stdlib) {
  is_stdlib = true;
  if (funcall->child(0)->getString() == "print") {
    return inferPrint(funcall, func);
  }

  if (funcall->child(0)->getString() == "len") {
    return inferLen(funcall, func);
  }

  if ((funcall->child(0)->getString() == "read_string") ||
      (funcall->child(0)->getString() == "read_int") ||
      (funcall->child(0)->getString() == "read_real") ||
      (funcall->child(0)->getString() == "read_bool")) {
    return inferRead(funcall);
  }

  is_stdlib = false;
  return NULL;
}

/* accepts a node as a parameter and returns true if that node 
 * cannot be inside a lambda, false otherwise */
bool noLambdaParents(Node* node) {
  NodeKind k = node->kind();

  if (!k || k == NODE_ASSIGN || k == NODE_LOCK 
      || k == NODE_BACKGROUND 
      || k == NODE_PARALLEL || k == NODE_WHILE || k == NODE_PARFOR 
      || k == NODE_FOR || k == NODE_ELIF_CLAUSE || k == NODE_IF) {

    return true;
  }
  return false;
}

/* takes a node as a parameter and returns the innermost lambda that
 * contains it, or null if it is not contained in a lambda */
Node* nextLambda(Node* startNode) {

  Node* curNode = startNode->getParent();

  /* keep going until you reach a node that can't be contained
   * in a lambda expression. */
  while (!noLambdaParents(curNode)) {
    /* if we reached a lambda, return it */
    if (curNode->kind() == NODE_LAMBDA) {
      return curNode;
    }
    /* otherwise, go up one */
    curNode = curNode->getParent();
  }
  /* if we got here, than we did not find
   * a containing lambda, and we reached a node
   * which cannot be contained in a lambda expression */
  return NULL;
}


/* look up parent class */
Node* getClassNode(Node* node){
  if (node->kind() == NODE_CLASS){
    return node;
  } else if (node->kind() == NODE_TOPLEVEL_LIST) { 
    return NULL;
  } else {
    return getClassNode(node->getParent());
  }

}

/* find identifier */
DataType* findIdType(Node* expr, Node* func) {
  /* check if it's a lambda param first */
  /* look for lambdas first */
  Node* lambda = nextLambda(expr);
  while (lambda) {
    /* if we found the identifier, return it's type */
    if(lambda->hasSymbol(expr->getString())) {
      return lambda->lookupSymbol(
          expr->getString(), lambda->getLine()).getType();     
    }

    /* otherwise, go to the next lambda up */
    lambda = nextLambda(expr);
  } 

  /* if not a lambda, see if it's local to the function */
  if (func->hasSymbol(expr->getString())){

    /* look it up and return that type */
    Symbol sym = func->lookupSymbol(expr->getString(), expr->getLine());
    return sym.getType();

  /* if it is in a class, check there for a member var*/
  } else if (getClassNode(expr) 
      && classes[getClassNode(expr)->getString()].hasMember(expr->getString())) {

    Symbol sym = classes[getClassNode(expr)->getString()].getMember(expr->getString()); 
    return sym.getType();

  /* if it is in a class, check there for a method */
  } else if (getClassNode(expr) 
      && classes[getClassNode(expr)->getString()].hasMethodNamed(expr->getString())) {

    /* get all the methods */
    return classes[getClassNode(expr)->getString()].getMethods(expr->getString()); 

  /* next check for globals/constants */
  } else if (globals.count(expr->getString()) > 0) {
     
    /* look it up and return that type */
    Symbol sym = globals[expr->getString()];
    return sym.getType();

  /* next check if it's the name of a free function */
  } else if (functions.hasFuncNamed(expr->getString())) {

    /* look it up and return that type */
    return functions.getFunctionsNamed(expr->getString());

  } else {
    return NULL;
  }
}

/* infer the types of an expression, and also return the type */
DataType* inferExpressionPrime(Node* expr, Node* func) {

  if (!expr) return NULL;

  /* the left hand side, right hand side, and result used below */
  DataType *lhs, *rhs;

  /* switch on the type of expression */
  switch (expr->kind()) {
    case NODE_ASSIGN: {
                        /* get the type of the right hand side */
                        rhs = inferExpression(expr->child(1), func);

                        /* if the left hand side is an identifier... */
                        if (expr->child(0)->kind() == NODE_IDENTIFIER) {
                          /* check if it exists */
                          /* check this function first */
                          if (func->hasSymbol(expr->child(0)->getString())){
                            /* set lhs equal to the type of the existing local identifier */
                            lhs = func->lookupSymbol(expr->child(0)->getString(), 
                                expr->getLine()).getType();


                            /* check globals next */
                          } else if (globals.count(expr->child(0)->getString()) > 0) {

                            /* get the symbol */
                            Symbol sym = globals.find(expr->getString())->second;

                            /* if it is a constant ... */
                            if (sym.isConst()){

                              throw Error("Cannot assign to Constant.", expr->getLine());
                            }

                            /*otherwise, set the lhs to the type of the existing global */
                            lhs = sym.getType();

                          /* make sure it is not the name of a class */
                          } else if (classes.count(expr->child(0)->getString())) {

                            throw Error("Cannot assign to class.", expr->getLine());

                          /* make sure it's not the name of a free function */   
                          } else if (functions.hasFuncNamed(expr->child(0)->getString())) {

                            throw Error("Cannot assign to free function.", expr->getLine());

                          /* if we end up here, then it doesn't exist yet */
                          /* make sure the right side wasnt {} or [] because
                           * we don't do type inference on these */
                          } else if (rhs->isEmptyContainerType()) {

                            throw Error("Cannot infer subtype of empty list/dictionary."
                                , expr->getLine());

                          /* if it doesn't exist and the type is inferable... */ 
                          } else {

                            /* infer it! */ 
                            lhs = rhs;
                            func->insertSymbol(*new Symbol(expr->child(0)->getString(),
                                  lhs,expr->child(0)->getLine()));
                          } 

                        /* if it's not directly and identifier.. */
                        } else {

                          /* get the type of the left hand side */
                          lhs = inferExpression(expr->child(0), func);

                          /* if there is an index on the left... */
                          if (expr->child(0)->kind() == NODE_INDEX) {
                            /* then check for immutable types */  
                            DataTypeKind assignKind = expr->child(0)->child(0)->type()->getKind();
                            if(assignKind == TYPE_TUPLE || assignKind == TYPE_STRING){
                              throw Error("Cannot assign to immutable types (string or tuple).",
                                  expr->getLine());
                            }
                          }
                        }

                        /* make sure both sides are the same type */
                        if(*rhs != *lhs) {
                          throw Error("Assignment of incompatible types.", expr->getLine()); 
                        }

                        /* return the type of the rhs */
                        return rhs;
                        break;
                      }

    case NODE_OR:
    case NODE_AND:
                      /* check that both children are bools */
                      lhs = inferExpression(expr->child(0), func);
                      rhs = inferExpression(expr->child(1), func);
                      if ((lhs->getKind() != TYPE_BOOL) || (rhs->getKind() != TYPE_BOOL)) {
                        throw Error("Only bool values may be used with and/or",
                            expr->getLine());
                      }
                      /* the result is a bool as well */
                      return new DataType(TYPE_BOOL);
    case NODE_LT:
    case NODE_LTE:
    case NODE_GT:
    case NODE_GTE:
                      /* check that both sides have the same type
                       * TODO at some point add in int->real promotion */
                      lhs = inferExpression(expr->child(0), func);
                      rhs = inferExpression(expr->child(1), func);
                      if (*lhs != *rhs) {
                        cout << "A = " << typeToString(lhs) << endl;
                        cout << "B = " << typeToString(rhs) << endl;
                        throw Error("Only matching types can be compared", expr->getLine());
                      }

                      /* check that they are strings, ints, or reals */
                      if (lhs->getKind() != TYPE_STRING 
                          && lhs->getKind() != TYPE_REAL && lhs->getKind() != TYPE_INT) {
                        throw Error("This comparison invalid on given type.", expr->getLine());
                      }

                      /* the result is a bool */
                      return new DataType(TYPE_BOOL);

    case NODE_EQ:
    case NODE_NEQ:
                      /* check that both sides have the same type
                       * TODO at some point add in int->real promotion */
                      lhs = inferExpression(expr->child(0), func);
                      rhs = inferExpression(expr->child(1), func);
                      if (*lhs != *rhs) {
                        cout << "A = " << typeToString(lhs) << endl;
                        cout << "B = " << typeToString(rhs) << endl;
                        throw Error("Only matching types can be compared", expr->getLine());
                      }

                      /* the result is a bool */
                      return new DataType(TYPE_BOOL);

    case NODE_NOT:
                      /* check that the operand is bool */
                      lhs = inferExpression(expr->child(0), func);
                      if (lhs->getKind() != TYPE_BOOL) {
                        throw Error("Operand of not must be a bool", expr->getLine());
                      }
                      return new DataType(TYPE_BOOL);

    case NODE_BITXOR:
    case NODE_BITAND:
    case NODE_BITOR:
    case NODE_SHIFTL:
    case NODE_SHIFTR:
                      /* check that both operands are integers */
                      lhs = inferExpression(expr->child(0), func);
                      rhs = inferExpression(expr->child(1), func);

                      if ((lhs->getKind() != TYPE_INT) || (rhs->getKind() != TYPE_INT)) {
                        throw Error("Operands to bitwise operator must be integer",
                            expr->getLine());
                      }

                      /* returns an integer back */
                      return new DataType(TYPE_INT);

    case NODE_BITNOT:
                      /* check that the operand is an int */
                      lhs = inferExpression(expr->child(0), func);
                      if (lhs->getKind() != TYPE_INT) {
                        throw Error("Operand to bitwise not must be an integer",
                            expr->getLine());
                      }
                      return new DataType(TYPE_INT);

    case NODE_PLUS:
    case NODE_MINUS:
    case NODE_TIMES:
    case NODE_DIVIDE:
    case NODE_MODULUS:
    case NODE_EXP:
                      /* check that both operands match and that they are int/real
                       * TODO at some point add in int->real promotion... */
                      lhs = inferExpression(expr->child(0), func);
                      rhs = inferExpression(expr->child(1), func);

                      if (*lhs != *rhs) {
                        throw Error("In binary operator, the types " + typeToString(lhs) +
                            " and " + typeToString(rhs) + " are not compatible",
                            expr->getLine());
                      }

                      if ((lhs->getKind() != TYPE_INT) && (rhs->getKind() != TYPE_REAL)) {
                        /* special case: adding strings and vectors is OK */
                        if ((expr->kind() == NODE_PLUS) && ((lhs->getKind() == TYPE_STRING) ||
                              (lhs->getKind() == TYPE_VECTOR))) {
                          /* s'alright */
                        } else {
                          throw Error("Numeric type required", expr->getLine());
                        }
                      }

                      /* if it's a vector, we need to copy the type! */
                      if (lhs->getKind() == TYPE_VECTOR) {
                        DataType* vec = new DataType(TYPE_VECTOR);

                        /* copy subs all the way */
                        DataType* sub = &(*(lhs->subtypes))[0];
                        DataType* ptr = vec;
                        while (sub) {
                          /* set current one */
                          ptr->subtypes->push_back(*sub);

                          /* move to next */
                          sub = &(*(sub->subtypes))[0];
                          ptr = &(*(ptr->subtypes))[0];
                        }

                        return vec;
                      }

                      /* return the same type back */
                      return new DataType(lhs->getKind());

    case NODE_INDEX: {
                       /* check children */
                       lhs = inferExpression(expr->child(0), func);
                       rhs = inferExpression(expr->child(1), func);

                       /* get the kind of lhs*/ 
                       DataTypeKind kind = lhs->getKind();

                       /* return the type of the container's values */
                       /* dictionaries */
                       if (kind == TYPE_DICT) {
                        /* check the key type */
                        if ((*(lhs->subtypes))[0] != *rhs) {
                          throw Error("Key has incompatible type.", expr->getLine());
                        }
                        return &(*(lhs->subtypes))[1]; 

                       /* tuples */
                       } else if (kind == TYPE_TUPLE) {
                         /* check the index type */
                         if (rhs->getKind() != TYPE_INT){
                          throw Error("Tuple index must be an integer.", expr->getLine());
                         }
                         /* make sure the index is in range */  
                         if ((unsigned long) expr->child(1)->getInt() < lhs->subtypes->size()){
                           /* get return the type of the index */
                           return &(*(lhs->subtypes))[expr->child(1)->getInt()]; 
                         /* if it isn't in range */
                         } else {
                           /* complain! */
                           throw Error("Tuple index out of range.", expr->getLine());
                         }

                       /* vectors */
                       } else if (kind == TYPE_VECTOR) {
                         /* check the index type */
                         if (rhs->getKind() != TYPE_INT){
                          throw Error("Vector index must be an integer.", expr->getLine());
                         }
                         return &(*(lhs->subtypes))[0]; 

                       /* strings */
                       } else if (kind == TYPE_STRING) {
                         /* check the index type */
                         if (rhs->getKind() != TYPE_INT){
                          throw Error("String index must be an integer.", expr->getLine());
                         }
                         return lhs; 

                       /* otherwise it isn't an indexable type */
                       } else {
                         throw Error("Index performed on unindexable type.", expr->getLine());
                       }

                     }

    case NODE_VECRANGE: {
                          lhs = inferExpression(expr->child(0), func); 
                          rhs = inferExpression(expr->child(1), func); 

                          /*make sure the types are both ints */
                          if (lhs->getKind() != TYPE_INT || lhs->getKind() != TYPE_INT) {
                            throw Error("Cannot create range with types other than INT."
                                , expr->getLine());
                          }

                          /* a vecrange can only possibly be a vector of ints */
                          DataType* t = new DataType(TYPE_VECTOR);
                          t->subtypes->push_back(*new DataType(TYPE_INT));
                          return t;
                        }

    case NODE_FUNCALL: {
                         /* check for stl functions */
                         bool is_stdlib;
                         lhs = inferStdlib(expr, func, is_stdlib); 
                         if (is_stdlib) {
                           return lhs;
                         }

                         /* if it's not an stl function */
                         /* infer the identifier */
                         lhs = inferExpression(expr->child(0),func);
                         /* make an empty tuple type for the params */
                         DataType* rhsParams = new DataType(TYPE_TUPLE);
                         /* if there are arguments... */
                         if (expr->numChildren() > 1) {
                           /* add them to the tuple */
                           buildParamTupleType(rhsParams,expr->child(1),func);
                         } 

                         /* make sure that we found a matching function */
                         /* if we have a single function... */
                         if (lhs->getKind() == TYPE_FUNCTION){
                           /* make sure it has the right params */
                           if ((*(lhs->subtypes))[0] == *rhsParams){
                             return lhs;
                           }
                           /* if we have multiple possibilities */
                         } else if (lhs->getKind() == TYPE_OVERLOAD) {
                           /* check each of them */
                           for (long unsigned int i = 0; i < lhs->subtypes->size(); i++) {
                             /* if it matches, return it */
                             if ((*(lhs->subtypes))[i] == *rhsParams){
                               return &((*(lhs->subtypes))[i]);
                             }
                           }
                         }
                         /* if we get here, then we either haven't found any matches,
                          * or the ones that we have found don't accept the correct arguments */
                         throw Error("No matching function.", expr->getLine()); 
                       }

    case NODE_ACTUAL_PARAM_LIST:
                       throw Error("inferExpression: should not be a param list here");
                       break;

    case NODE_IDENTIFIER: { /* first check if it already has a type (this happens for declarations) */
                            if (expr->type()) {
                              /* if it is a classType, make sure the class exists */
                              if (expr->type()->getKind() == TYPE_CLASS 
                                  && !classes.count(*(expr->type()->className))) {
                                throw Error("Class does not exist.", expr->getLine());
                              }

                              /* if the identifier already exists... */
                              if(findIdType(expr, func)) {
                                /* complain! */
                                throw Error("The identifier '" + expr->getString() 
                                    + "' already exists.",expr->getLine());
                              }

                              /* if we make it here, then just return the type it already has */
                              return expr->type();
                            } 
                            /* otherwise, if it doesn't already have a type... */
                            /* if the id already exists, get its type */
                            DataType* type = findIdType(expr, func);

                            /* if we didn't find it... */
                            if (!type) {
                              /* complain! */
                              throw Error("Reference to non-existent identifier '" 
                                  + expr->getString() + "'.", expr->getLine());
                            } 

                            /* otherwise, return the type */
                            return type;

                          }

                          /* return these types */
    case NODE_INTVAL:
                          return new DataType(TYPE_INT);
    case NODE_REALVAL:
                          return new DataType(TYPE_REAL);
    case NODE_BOOLVAL:
                          return new DataType(TYPE_BOOL);
    case NODE_STRINGVAL:
                          return new DataType(TYPE_STRING);
    case NODE_NONEVAL:
                          return new DataType(TYPE_NONE);
    case NODE_VECVAL: {
                        DataType* dt = new DataType(TYPE_VECTOR);
                        Node * currNode = expr;
                        /* traverse the subtree of vecvals */
                        while(currNode && currNode->numChildren() > 0){

                          DataType* elemType = inferExpression(currNode->child(0),func);
                          /* if this is the first element, add the subtype */
                          if(dt->subtypes->size() == 0) {
                            (dt->subtypes)->push_back(*elemType); 
                            /* if there is a previous subtype, make sure they match */
                          } else if(dt->subtypes->size() == 1 
                              && ((*(dt->subtypes))[0]) != *elemType){

                            throw Error("Mismatched vector types", expr->getLine());
                          }
                          /* set current node to the next one */
                          currNode = currNode->child(1);
                        }
                        return dt;
                      }

    case NODE_DICTVAL: {
                         DataType* dt = new DataType(TYPE_DICT);
                         Node * currNode = expr;
                         /* traverse the subtree of dictvals */
                         while(currNode && currNode->numChildren() > 0) {
                           DataType* keyType = inferExpression(currNode->child(0),func);
                           DataType* valType = inferExpression(currNode->child(1),func);
                           /* if this is the first element, add the subtypes */
                           if(dt->subtypes->size() == 0) {
                             dt->subtypes->push_back(*keyType); 
                             dt->subtypes->push_back(*valType); 
                             /* if there are previous subtypes, make sure they match */
                           } else if(dt->subtypes->size() == 2 
                               && (((*(dt->subtypes))[0] != *keyType)
                                 || ((*(dt->subtypes))[1] != *valType))){

                             throw Error("Mismatched key/value types", expr->getLine());
                           }
                           /* set current node to the next one */
                           currNode = currNode->child(2);
                         }

                         return dt;
                       }

    case NODE_TUPVAL:{
                       DataType* dt = new DataType(TYPE_TUPLE);
                       Node* currNode = expr;
                       /* traverse the subtree of vecvals */
                       while(currNode && currNode->numChildren() > 0) {

                         DataType* elemType = inferExpression(currNode->child(0),func);

                         /* add the subtype */
                         dt->subtypes->push_back(*elemType); 

                         /* set current node to the next one */
                         currNode = currNode->child(1);
                       }

                       return dt;
                     }

    case NODE_LAMBDA:{ 
                       /* make sure that any classes that are referred to
                        * actually exist */
                       checkClassTypes(expr->child(0));
                       /* add the params to the symtable */
                       inferParams(expr);
                       /* make a new DataType for this lambda */
                       DataType* type = new DataType(TYPE_FUNCTION);
                       /* add any param types as subtypes */
                       DataType* paramTypes = new DataType(TYPE_TUPLE);
                       for (std::map<std::string, Symbol>::iterator it = expr->symtable->begin(); 
                           it != expr->symtable->end(); it ++){

                         paramTypes->subtypes->push_back(*(it->second.getType()));
                       }
                       /* add the params as a subtype */
                       type->subtypes->push_back(*paramTypes);

                       /* infer the the return type */
                       if (expr->numChildren() > 1) {
                         type->subtypes->push_back(*inferExpression(expr->child(1),func));
                       } else {
                         type->subtypes->push_back(*inferExpression(expr->child(0),func));
                       }

                       return type;
                     }

    case NODE_DOT:{
                    lhs = inferExpression(expr->child(0),func);  
                    /* check that class exists */
                    if (!lhs->className || !classes.count(*lhs->className)){
                      throw Error("Class does not exist.", expr->getLine());
                      /* check that class has member var */
                    } else if (!classes[*lhs->className]
                        .hasMember(expr->child(1)->getString())){
                      throw Error("Class does not contain specified member variable."
                          , expr->getLine());
                    }

                    /* return the type of the member variable */
                    return classes[*lhs->className].getMember(expr->child(0)->getString()).getType();
                  }
    case NODE_SELF: {
                      /* return parent class type*/
                      Node* classNode = getClassNode(expr);
                      /* throw error if no parent class found */
                      if (!classNode) {
                        throw Error("Reference to 'self' outside of class.", 
                            expr->getLine());
                      }

                      return classNode->type();

                    }

    case NODE_METHOD_CALL: {
                             lhs = inferExpression(expr->child(0),func);  
                             /* infer the tuple_type of the actual params */
                             DataType* rhsParams = new DataType(TYPE_TUPLE);

                             /* if there are actual params... */
                             if (expr->child(1)->numChildren() > 1) {
                               /* add them to the tuple_type */
                               buildParamTupleType(rhsParams,expr->child(1)->child(1),func);
                             }

                             /* check that class exists */
                             if (!lhs->className || !classes.count(*lhs->className)){
                               throw Error("Class does not exist.", expr->getLine());

                               /* check that class has method */
                             } else if (!classes[*lhs->className]
                                 .hasMethod(rhsParams, expr->child(1)->getString())){

                               throw Error("Class does not contain specified member variable."
                                   , expr->getLine());
                             }

                             /* return the return type of the method */
                             return classes[*lhs->className]
                               .getMethod(rhsParams, expr->child(0)->getString())->type();
                           }

    case NODE_DECLARATION: {

                           } 

    default:
                           cout << expr->kind() << endl;
                           throw Error("inferExpression: unknown node type");
                           break;
  }

  throw Error("inferExpression: unhandled type");
}

/* infer an expression and assign it to the node */
DataType* inferExpression(Node* expr, Node* func) {
  /* do the inference */
  DataType* t = inferExpressionPrime(expr, func);

  /* assign it into this node */
  expr->setDataType(t);

  /* return it */
  return t;
}

/* infer/ type check function for tasks and locks */
void checkMuTasks(Node* block, Node* func) {
  /* is it a task or a mutex? */
  DataTypeKind kind;
  switch (block->kind()) {
    /* task */
    case NODE_BACKGROUND:
    case NODE_WAIT:
      kind = TYPE_TASK; 
      break;
      /* mutex */
    case NODE_LOCK: 
      kind = TYPE_MUTEX; 
      break;
      /* some other type (this should never happen) */
    default:
      throw Error("Something's wrotten in the state of Denmark!", 
          block->getLine());
  } 

  /* if there are two children or it's a wait block, it's named */
  if (block->child(1) || block->kind() == NODE_WAIT){
    /* check if the identifier exists */
    DataType* type = NULL;
    type = findIdType(block->child(0), func);

    /* if the identifier doesn't exist yet 
     * and it's not a wait node*/ 
    if (!type && block->kind() != NODE_WAIT){
      /* add to this function's symtable */
      type = new DataType(kind);
      func->insertSymbol(Symbol(block->child(0)->getString(), 
            type, block->child(0)->getLine()));  

      /* if the identifier doesn't exist and it
       * is a wait node*/
    } else if (!type) {
      throw Error("Cannot wait for task that has not been created",
          block->child(0)->getLine());
    }

    /* if the type is wrong... */
    if (type->getKind() != kind){
      throw Error("Task or mutex identifier has improper type.", 
          block->child(0)->getLine());
    }

    /* set the type */
    block->child(0)->setDataType(type);

    /* if there is a block ... */
    if (block->child(1)) {
      /* check the block */
      inferBlock(block->child(1), func);
    }

    /* otherwise, it's an unnamed task/lock */
  } else{

    /* check the sub-block */
    inferBlock(block->child(0), func);
  }
}

/* infer types in a block and add them into the symbol table */
void inferBlock(Node* block, Node* func) {
  if (!block) return;

  /* switch on the different types */
  switch (block->kind()) {
    case NODE_STATEMENT:
      /* handle both children */
      inferBlock(block->child(0), func);
      inferBlock(block->child(1), func);
      break;
    case NODE_RETURN: {
                        /* infer the expression */
                        DataType* ret;
                        if (block->child(0)) {
                          ret = inferExpression(block->child(0), func);
                        } else {
                          ret = new DataType(TYPE_NONE);
                        }

                        /* check that it matches the return type */
                        if (*ret != func->type()->subtypes->back()) {
                          throw Error("Return value type does not match function's declared type",
                              block->getLine());
                        }
                        break;
                      }
    case NODE_IF: {
                    /* infer the type of the expression */
                    DataType* cond = inferExpression(block->child(0), func);

                    /* check that it is a BOOL */
                    if (cond->getKind() != TYPE_BOOL) {
                      throw Error("if condition must be a bool", block->getLine());
                    }

                    /* infer both the then and else blocks */
                    inferBlock(block->child(1), func);
                    inferBlock(block->child(2), func);
                    break;
                  }
    case NODE_ELIF:
                  /* check the first child which is the first elf clause */
                  inferBlock(block->child(0), func);

                  /* check the second child which is the chain of the rest of the elifs */
                  inferBlock(block->child(1), func);

                  /* check the third child which is the else clause (maybe NULL) */
                  inferBlock(block->child(2), func);
                  break;
    case NODE_ELIF_CHAIN:
                  /* check the clause on the left and the chain on the right */
                  inferBlock(block->child(0), func);
                  inferBlock(block->child(1), func);
                  break;
    case NODE_ELIF_CLAUSE: {
                             /* check the expression on the left */
                             DataType* cond = inferExpression(block->child(0), func);

                             /* check that it is a BOOL */
                             if (cond->getKind() != TYPE_BOOL) {
                               throw Error("elif condition must be a bool", block->getLine());
                             }

                             /* check the statements on the right */
                             inferBlock(block->child(1), func);
                             break;
                           }
    case NODE_WHILE: {
                       /* infer the type of the expression */
                       DataType* cond = inferExpression(block->child(0), func);

                       /* check that it is a BOOL */
                       if (cond->getKind() != TYPE_BOOL) {
                         throw Error("while condition must be a bool", block->getLine());
                       }

                       /* infer the body */
                       inferBlock(block->child(1), func);
                       break;
                     }
    case NODE_PARFOR:
    case NODE_FOR: {
                     /* infer the type of the expression */
                     DataType* expr_type = inferExpression(block->child(1), func);

                     /* make sure it is some type of vector */
                     if (expr_type->getKind() != TYPE_VECTOR) {
                       throw Error("for expression must have vector type", block->getLine());
                     }

                     /* put the identifier in the func */
                     func->insertSymbol(Symbol(block->child(0)->getString(),
                           &(*(expr_type->subtypes))[0], block->getLine()));

                     /* set the type of the node too */
                     block->child(0)->setDataType(&(*(expr_type->subtypes))[0]);

                     /* check the block under this */
                     inferBlock(block->child(2), func);
                     break;
                   }
    case NODE_PARALLEL:
                   inferBlock(block->child(0), func);
                   break;
    case NODE_BACKGROUND: 
    case NODE_LOCK:
    case NODE_WAIT:
                   checkMuTasks(block, func); 
                   break;
                   /* these require no work... */
    case NODE_PASS:
    case NODE_BREAK:
    case NODE_CONTINUE:
                   break;

    default:
                   /* handle it as an expression */
                   inferExpression(block, func);
  }
}

/* infer types based on a single function */
void inferParams(Node* node) {
  /* only works on functions */
  if (!node || ((node->kind() != NODE_FUNCTION) 
        && (node->kind() != NODE_LAMBDA))) {
    return;
  }

  /* if there are parameters */
  if (node->numChildren() > 1) {
    /* add the parameters into the table */
    addParams(node->child(0), node);
  }

}


/* infer a global/const definition */
void inferGlobal(Node* node, bool isConst = false) {

  /* name for errors */
  string varType;
  isConst ? varType = "Constant" : varType = "Global";

  /* check if this symbol exists already, (it shouldn't)*/
  if (globals.count(node->child(0)->getString()) > 0
      || functions.hasFuncNamed(node->child(0)->getString())
      || classes.count(node->child(0)->getString()) > 0) {

    throw Error(varType + " '" + node->child(0)->getString() +
        "' has been defined.");
  }

  /* if there is an assignment */
  if (node->child(1)){

    /* get the type of the right hand side */
    DataType* rhs = inferExpression(node->child(1), NULL);

    /* if the left hand side has a declared type.. */
    if (node->child(0)->type() ){
      /* check that the types match */
      if (*(node->child(0)->type()) != *rhs) {
        throw Error ("Assignment of unmatched types.",node->getLine());
      }
      /* if there is no declared type */
    } else {
      /* infer the type from the right side */
      node->setDataType(rhs);

    }

    /* set the node type */
    node->child(0)->setDataType(rhs);
  }
  /* add it in */
  globals.insert(pair<string, Symbol>(
        node->child(0)->getString(),
        Symbol(node->child(0)->getString(), node->type(), node->getLine())));
}

void inferGlobals(Node* node){

  /* infer each function */
  if (node && (node->kind() == NODE_TOPLEVEL_LIST)) {
    /* infer child 0 based on its kind */
    if (node->child(0)->kind() == NODE_CONST) {
      inferGlobal(node->child(0), true);
    } else if (node->child(0)->kind() == NODE_GLOBAL) {
      inferGlobal(node->child(0));
    } 

    /* recursively infer any other functions */
    inferGlobals(node->child(1));
  }
}

/* add stl functions to the list of globals */
void addStls() {

  string stls [] = {"len", "read_string", "read_int", 
    "read_real", "read_bool", "print"};

  for (unsigned long i = 0; i < sizeof(stls)/sizeof(stls[0]); i++) {
    /* add them to the globals */
    DataType type(TYPE_FUNCTION);

    globals.insert(pair<string, Symbol>( "len",
          Symbol(stls[i], &type, 0, true)));
  }
}

/* Given a class node, this function adds the member vars 
 * into the class map (Cannot type check yet as this class 
 * may have members that are dependent on
 * classes that have not been seen yet.)*/
void addMembers(ClassContext* context, Node* node) {
  /* add this part */
  if (node->kind() == NODE_IDENTIFIER) {
    context->addMember(Symbol(node->getString(), node->type(), 
          node->getLine())); 
  } else if (node->kind() == NODE_CLASS_PART) {
    /* recursively add other parts */
    addMembers(context, node->child(0));
    if(node->child(1)){
      addMembers(context, node->child(1));
    }
  }
}

/* Given a class context, removes any init functions
 * from its list of methods and adds them as 
 * constructors to the lists of free functions
 * (also adds a default constructor if one does not
 * exist) */
void initSquared(ClassContext context) {

  /* remove the init functions */
  std::map<std::string,Node*> inits 
    = context.removeInits();
  
  bool hasDefault = false;
  DataType* type = new DataType(TYPE_CLASS);
  *(type->className) = context.getName();

  /* loop through the inits*/
  for (std::map<std::string, Node*>::iterator it = inits.begin(); 
    it != inits.end(); it ++) {

    /* update the return types to this class's type*/
    (*(it->second->type()->subtypes))[(*(it->second->type()->subtypes)).size()-1] = *type;
    
    /* rename the functions and insert them */
    functions.insert(std::pair<string,Node*>(context.getName() 
          + it->first.substr((it->first).find_first_of("(")), it->second));

    /* if the function is a default constructor... */
    if (it->first == "init()") {
      /* take note! */
      hasDefault = true;
    }
  }
  /* if there was no default constructor... */
  if (!hasDefault) {
    /* make one and add it! */
    Node* node = new Node(NODE_FUNCTION);
    node->setDataType(type);
    std::string key = context.getName() + "()";
    functions.insert(std::pair<string,Node*> (key, node)); 
  }
}

/* Makes initial pass through top levels to populate
 * class map*/
void initClass(Node* node) {

  /* First, we need to read in the the Classes. */ 
  if (node && (node->kind() == NODE_TOPLEVEL_LIST)) {
    if (node->child(0)->kind() == NODE_CLASS) { 
      /* check for duplicate class name */
      if (classes.count(node->child(0)->getString())) {
        throw Error("Duplicate class.", 
            node->child(0)->getLine());
      }
      /* check for stl names*/
      if (globals.count(node->child(0)->getString())) {
        throw Error("Cannot use stl function name for class name.", 
            node->child(0)->getLine());
      }

      /* create new ClassContext */
      ClassContext context(node->child(0)->getString());

      /* add any class parts */
      context.addMembers(node->child(0));
      context.addMethods(node->child(0));

      /* remove the init functions from the class' methods
       * and add them as globally available class constructors */
      initSquared(context);

      /* add to map of classes */
      classes[context.getName()] = context;
    }

    /* recursively infer any other functions */
    initClass(node->child(1));
  }
}

/* checks for the existence of any classes which are referred to */
void checkClassTypes(Node* node) {
  /* check return types */
  if (node->kind() == NODE_FUNCTION) {
    /* if the return type is a class, make sure that
     * it exists */
    if ((*(node->type()->subtypes)).back().getKind() == TYPE_CLASS 
        && !classes.count(*((*(node->type()->subtypes)).back().className))) {
      throw Error("Return type does not exist.", node->getLine());  
    } 

    /* if it has params... */
    if (node->numChildren() > 1) {
      /* check those too */
      checkClassTypes(node->child(0));
    }

  } else if (node->kind() == NODE_FORMAL_PARAM_LIST) {
    checkClassTypes(node->child(0));
    checkClassTypes(node->child(1));

  } else if (node->kind() == NODE_DECLARATION) {
    if (node->type()->getKind() == TYPE_CLASS 
        && !classes.count(*(node->type()->className))){
      throw Error("Class does not exist.", node->getLine());  
    } 
  }
}

void inferFunction(Node* node){
  /* make sure that this function does not share a name
   * with a global or a class */
  if (globals.count(node->getString())
      || classes.count(node->getString())) {

    throw Error("Function name is used elsewhere.", 
        node->getLine());
  }

  /* if there are params...*/ 
  if (node->numChildren( ) > 1) {
    checkClassTypes(node->child(0));
    inferBlock(node->child(1), node);
  } else {
    inferBlock(node->child(0),node);
  }
}



/* infer types for class declaration */
void inferClass(Node* node) {
  if (node->kind() == NODE_CLASS){
    if (functions.hasFuncNamed(node->getString())) {
      throw Error ("Class name cannot share free function identifier.", 
          node->getLine());
    }
    inferClass(node->child(0));
  } else if (node->kind() == NODE_CLASS_PART) {
    inferClass(node->child(0));  
    if(node->child(1)){
      inferClass(node->child(1));
    }
  } else if (node->kind() == NODE_IDENTIFIER) {
    checkClassTypes(node);

  } else if (node->kind() == NODE_FUNCTION) {
    inferFunction(node);
  } 
}

/* populates classes, free functions, and globals */
void initTypes(Node* node) {
  /* add all the stl functions to the global list
   * to reserve their names */
  addStls();

  /* add all the classes */
  initClass(node);

  /* add all the free functions */
  functions.build(node);

  /* add and type check/infer all the globals/constants */
  inferGlobals(node);
}

/* this function does type checking/type inference on a parse tree */
void inferTypes(Node* node) {

  /* infer each function */
  if (node && (node->kind() == NODE_TOPLEVEL_LIST)) {
    /* infer child 0 based on its kind */
    if (node->child(0)->kind() == NODE_FUNCTION) {
      inferFunction(node->child(0));
    } else if (node->child(0)->kind() == NODE_CLASS) { 
      inferClass(node->child(0));
    }
    /*TODO: open, import */

    /* recursively infer any other functions */
    inferTypes(node->child(1));
  }
}
