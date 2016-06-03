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
map<string, Symbol> globals;

/* prototypes */
void inferBlock(Node*, Node* );
void inferFunction(Node* , Node* parent =  NULL); 

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

DataTypeKind DataType::getKind() const { return kind; }

/* find the base type of a data type */
DataTypeKind baseType(DataType* t) {
  if (t->getKind() == TYPE_VECTOR)
    return baseType(&((*(t->subtypes))[0]));
  else
    return t->getKind();
}

/* compare two data types for equality */
bool operator==(const DataType& lhs, const DataType& rhs) {
  /* if they're not the same kind, fail */
  if (lhs.getKind() != rhs.getKind()) {
    return false;
  } 

  /* if they're vectors, recursively ensure the subtypes match */
  if (lhs.getKind() == TYPE_VECTOR) {
    return (lhs.subtypes[0] == rhs.subtypes[0]);
  }
  
  /* for dictionaries, recursively check the key and value types */
  if (lhs.getKind() == TYPE_DICT) {
    return (lhs.subtypes[0] == rhs.subtypes[0])
      && (lhs.subtypes[1] == rhs.subtypes[1]);
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
      same = (lhs.subtypes[i] == rhs.subtypes[i]);
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

/* forward declaration needed as these are mutually-recursive */
DataType* inferExpression(Node* expr, Node* func, Node* lambda = NULL);

/* infer the types of a print call */
DataType* inferPrint(Node* pcall, Node* func) {
  /* just infer each expression, but we don't care what it is */

  /* get the first arg */
  Node* arg = pcall->child(0);

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
  if (funcall->numChildren() != 1) {
    throw Error("len function expects one argument", funcall->getLine());
  }

  /* infer the argument and capture its type */
  DataType* t = inferExpression(funcall->child(0), func);

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
  if (funcall->numChildren() > 0) {
    throw Error(funcall->getString() + " should not have any parameters",
        funcall->getLine());
  }

  /* get the return type right */
  if (funcall->getString() == "read_string") return new DataType(TYPE_STRING);
  if (funcall->getString() == "read_int") return new DataType(TYPE_INT);
  if (funcall->getString() == "read_real") return new DataType(TYPE_REAL);
  if (funcall->getString() == "read_bool") return new DataType(TYPE_BOOL);

  throw Error("This should not happen!", funcall->getLine());
}

/* this function checks if a function call is part of the standard library and
 * infers it */
DataType* inferStdlib(Node* funcall, Node* func, bool& is_stdlib) {
  is_stdlib = true;
  if (funcall->getString() == "print") {
    return inferPrint(funcall, func);
  }

  if (funcall->getString() == "len") {
    return inferLen(funcall, func);
  }

  if ((funcall->getString() == "read_string") ||
      (funcall->getString() == "read_int") ||
      (funcall->getString() == "read_real") ||
      (funcall->getString() == "read_bool")) {
    return inferRead(funcall);
  }

  is_stdlib = false;
  return NULL;
}

/* count the number of indices used in an index operation */
int countIndices(Node* idx, Node* func) {
  if (!idx) return 0;

  /* check the index for being an int */
  DataType* itype = inferExpression(idx->child(0), func);
  if (itype->getKind() != TYPE_INT) {
    throw Error("Index must be integral", idx->getLine());
  }

  /* return 1 for this, and recurse */
  return 1 + countIndices(idx->child(1), func);
}

/* count the number of dimensions in a data type */
int countDimensions(DataType* t) {
  if (t->getKind() == TYPE_VECTOR) {
    return 1 + countDimensions(&((*(t->subtypes))[0]));
  } else {
    return 0;
  }
}

/* find a function by name in the parse tree */
Node* findFunction(const string& name, int lineno) {
  Node* f = root;

  while (f && (f->numChildren() > 0)) {
    /* see if this node is the thing */
    if (f->child(0)->getString() == name) {
      return f->child(0);
    }

    /* move to the next */
    f = f->child(1);
  }

  throw Error("Function '" + name + "' not defined", lineno);
}

/* infer the function of a function call and check the types */
DataType* inferFuncall(Node* funcall, Node* func, Node* lambda) {
  /* check for stdlib functions */
  bool is_stdlib;
  DataType* t = inferStdlib(funcall, func, is_stdlib);
  if (is_stdlib) {
    return t;
  }

  /* find the function node this thing matches */
  Node* f = findFunction(funcall->getString(), funcall->getLine());

  /* if the call has no parameters, make sure the function doesn't expect any */
  if (funcall->numChildren() == 0) {
    if (f->numChildren() > 1) {
      throw Error("Funcion called with no parameters, but expects some",
          funcall->getLine());
    }

    /* return from here as the below code expects >= 1 param */
    return f->type();
  }

  /* for each param, and also for each expression, make sure they match up */
  Node* formal = f->child(0);
  Node* actual = funcall->child(0);

  int p = 1;
  while (formal && actual) {
    DataType *t1, *t2;

    /* it could be a list, or an actual param */
    if (actual->kind() == NODE_ACTUAL_PARAM_LIST) {
      t1 = inferExpression(actual->child(0), func);
    } else {
      t1 = inferExpression(actual, func);
    }

    /* same for formal */
    if (formal->kind() == NODE_FORMAL_PARAM_LIST) {
      t2 = formal->child(0)->type();
    } else {
      t2 = formal->type();
    }

    /* check for type mismatch */
    if (*t1 != *t2) {
      throw Error("Function '" + funcall->getString() + "' expected " +
          typeToString(t2) + " but was called with " +
          typeToString(t1) + " for parameter " + itoa(p),
          funcall->getLine());
    }
    p++;

    /* move on to the next one */
    if (formal->kind() == NODE_FORMAL_PARAM_LIST) {
      formal = formal->child(1);
    } else {
      formal = NULL;
    }

    /* move on to the next one */
    if (actual->kind() == NODE_ACTUAL_PARAM_LIST) {
      actual = actual->child(1);
    } else {
      actual = NULL;
    }
  }

  /* if there are any left, the arity mismatched */
  if (formal) {
    throw Error("Too few parameters to function '" + funcall->getString() + "'",
        funcall->getLine());
  } else if (actual) {
    throw Error(
        "Too many parameters to function '" + funcall->getString() + "'",
        funcall->getLine());
  }

  /* return the type of function itself */
  return f->type();
}

/* infer the types of an expression, and also return the type */
DataType* inferExpressionPrime(Node* expr, Node* func, 
    Node* lambda) {

  if (!expr) return NULL;

  /* the left hand side, right hand side, and result used below */
  DataType *lhs, *rhs;

  /* switch on the type of expression */
  switch (expr->kind()) {
    case NODE_ASSIGN: {
                        /* get the type of the right hand side */
                        rhs = inferExpression(expr->child(1), func);

                        /* if the left hand side is an identifier... */
                        if (expr->child(0)->kind() == NODE_IDENTIFIER){
                          /* check if it exists */
                          /* check globals first */
                          if (globals.count(expr->getString()) > 0) {
                            
                            /* get the symbol */
                            Symbol sym = globals.find(expr->getString())->second;

                            /* if it is a constant ... */
                            if (sym.isConst()){

                              throw Error("Cannot assign to Constant.", expr->getLine());
                            }

                            /*otherwise, set the lhs to the type of the existing global */
                            lhs = sym.getType();
                            
                          /* if it wasn't a global, check this function */
                          } else if (func->hasSymbol(expr->child(0)->getString())){
                            /* set lhs equal to the type of the existing local identifier */
                            lhs = func->lookupSymbol(expr->child(0)->getString(), 
                                expr->getLine()).getType();
                          /* if we end up here, then it doesn't exist yet */
                          } else {
                            /* so add it! */
                            lhs = rhs;
                            func->insertSymbol(*new Symbol(expr->child(0)->getString(),
                                  lhs,expr->child(0)->getLine()));
                          } 

                        /* if it's not directly and identifier.. */
                        } else {

                          /* get the type of the left hand side */
                          lhs = inferExpression(expr->child(0), func);

                          /* if there is an index on the left... */
                          if (expr->child(0)->kind() == NODE_INDEX){
                            /* then check for immutable types */  
                            DataTypeKind assignKind = expr->child(0)->child(0)->type()->getKind();
                            if(assignKind == TYPE_TUPLE || assignKind == TYPE_STRING){
                              throw Error("Cannot assign to immutable types (string or tuple).",
                                  expr->getLine());
                            }
                          }

                        }

                        /* make sure both sides are the same type */
                        if(*rhs != *lhs){
                          throw Error("Assignment of incompatible types.", expr->getLine()); 
                        }

                        /* return the type of the rhs */
                        return rhs;
                        break;
                      }

    case NODE_OR:
    case NODE_AND:
                      /* check that both children are bools */
                      lhs = inferExpression(expr->child(0), func, lambda);
                      rhs = inferExpression(expr->child(1), func, lambda);
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
    case NODE_EQ:
    case NODE_NEQ:
                      /* check that both sides have the same type
                       * TODO at some point add in int->real promotion */
                      lhs = inferExpression(expr->child(0), func, lambda);
                      rhs = inferExpression(expr->child(1), func, lambda);
                      if (*lhs != *rhs) {
                        cout << "A = " << typeToString(lhs) << endl;
                        cout << "B = " << typeToString(rhs) << endl;
                        throw Error("Only matching types can be compared", expr->getLine());
                      }

                      /* the result is a bool */
                      return new DataType(TYPE_BOOL);

    case NODE_NOT:
                      /* check that the operand is bool */
                      lhs = inferExpression(expr->child(0), func, lambda);
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
                      lhs = inferExpression(expr->child(0), func, lambda);
                      rhs = inferExpression(expr->child(1), func, lambda);

                      if ((lhs->getKind() != TYPE_INT) || (rhs->getKind() != TYPE_INT)) {
                        throw Error("Operands to bitwise operator must be integer",
                            expr->getLine());
                      }

                      /* returns an integer back */
                      return new DataType(TYPE_INT);

    case NODE_BITNOT:
                      /* check that the operand is an int */
                      lhs = inferExpression(expr->child(0), func, lambda);
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
                      lhs = inferExpression(expr->child(0), func, lambda);
                      rhs = inferExpression(expr->child(1), func, lambda);

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
                      lhs = inferExpression(expr->child(0), func, lambda);
                      rhs = inferExpression(expr->child(1), func, lambda);

                      /* Do I exist (is my left child indexable)?*/ 
                      DataTypeKind kind = lhs->getKind();
                      if(!(kind == TYPE_VECTOR || kind == TYPE_TUPLE || kind == TYPE_DICT
                            || kind == TYPE_STRING)) {
                        throw Error("Index performed on unindexable type.", expr->getLine());
                      }

                      /* make sure the index type is correct (matches key type 
                       * for dictionaries, ints elsewhere)*/
                      if ((kind == TYPE_DICT && (*(lhs->subtypes))[0] != *rhs) 
                          || (kind != TYPE_DICT && rhs->getKind() != TYPE_INT)){
                          throw Error("Key has incompatible type.", expr->getLine());
                      } 

                      return rhs; 
                      
                     }
    case NODE_VECRANGE: {
                          /* a vecrange can only possibly be a vector of ints */
                          DataType* t = new DataType(TYPE_VECTOR);
                          t->subtypes->push_back(*new DataType(TYPE_INT));
                          return t;
                        }
    case NODE_FUNCALL:
                        return inferFuncall(expr, func, lambda);
    case NODE_ACTUAL_PARAM_LIST:
                        throw Error("inferExpression: should not be a param list here");
                        break;

    case NODE_IDENTIFIER: {
                            /* check if it's a global first */
                            if (globals.count(expr->getString()) > 0) {
                              Symbol sym = globals.find(expr->getString())->second;
                              return sym.getType();

                            /* if not, see if it's local to the function */
                            } else if (func->hasSymbol(expr->getString())){

                              /* look it up and return that type */
                              Symbol sym = func->lookupSymbol(expr->getString(), expr->getLine());
                              return sym.getType();

                            /* if it wasn't local or glabal, and this is called from a lambda,
                             * check for context local to the lambda declaration */
                            }
                            break;
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

    case NODE_VECVAL: {
                        DataType* dt = new DataType(TYPE_VECTOR);
                        dt->subtypes->push_back(*inferExpression(expr->child(0), func, lambda));

                        /* if there are more than one child, recurse on them too */
                        for (int i = 1; i < expr->numChildren(); i++) {
                          DataType* other = inferExpression(expr->child(i), func, lambda);

                          /* check that they match! */
                          if (*other != *dt) {
                            throw Error("Mismatched vector types", expr->getLine());
                          }
                        }

                        return dt;
                      }

    case NODE_LAMBDA: 
                      inferFunction(expr,func);

    default:
                      cout << expr->kind() << endl;
                      throw Error("inferExpression: unknown node type");
                      break;
  }

  throw Error("inferExpression: unhandled type");
}

/* infer an expression and assign it to the node */
DataType* inferExpression(Node* expr, Node* func, Node* lambda) {
  /* do the inference */
  DataType* t = inferExpressionPrime(expr, func, lambda);

  /* assign it into this node */
  expr->setDataType(t);

  /* return it */
  return t;
}

/* infer/ type check function for tasks and locks */
void checkMuTasks(Node* block, Node* func) {
  /* is it a task or a mutex? */
  DataTypeKind kind;
  switch (block->kind()){
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
    /* check if it's a global first */
    if (globals.count(block->child(0)->getString())) {
      type = (globals.find(block->child(0)->getString())->second).getType();

    /* if not, check if it's local */
    } else if (func->hasSymbol(block->child(0)->getString())) {
      type = func->lookupSymbol(block->child(0)->getString(), 
          block->getLine()).getType();

    /* if we get here, the identifier doesn't exist yet 
     * and it's not a wait node*/ 
    } else if (block->kind() != NODE_WAIT){
      /* add to this function's symtable */
      type = new DataType(kind);
      func->insertSymbol(Symbol(block->child(0)->getString(), 
            type, block->child(0)->getLine()));  
    
    /*if we get here, then it is a wait node and the identifier
     * doesn't exist yet*/
    } else {
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
    if (block->child(1)){
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
                        if (*ret != *(func->type())) {
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
void inferFunction(Node* node, Node* parent) {
  /* only works on functions */
  if (!node || ((node->kind() != NODE_FUNCTION) 
        && (node->kind() != NODE_LAMBDA))) {
    return;
  }

  /* if there is a block for parameters */
  if (node->numChildren() > 1) {
    /* add the parameters into the table */
    addParams(node->child(0), node);
    /* a proper function has a block */
    if(node->kind() == NODE_FUNCTION){
      inferBlock(node->child(1), node);
    /* lambda functions only have one expression */
    } else {
      inferExpression(node->child(1), parent,node);
    }
  } else {
    /* just infer the body of the function */
    if(node->kind() == NODE_FUNCTION) {
      inferBlock(node->child(0), node);
    } else {
      inferExpression(node->child(0), parent,node);
    }
  }
}

/* infer types for class declaration */
void inferClass(Node* node){

}

/* infer a global/const definition */
void inferGlobal(Node* node, bool isConst = false) {

  /* name for errors */
  string varType;
  isConst ? varType = "Constant" : varType = "Global";

  /* check if this symbol exists already, (it shouldn't)*/
  if (globals.count(node->child(0)->getString()) > 0
      || FunctionMap::hasFuncNamed(node->child(0)->getString())) {

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



/* this function does type checking/type inference on a parse tree */
void inferTypes(Node* node) {

  /* Traverse the tree and add all the function defs to the function map*/
  FunctionMap::build(node);

  /* infer each function */
  if (node && (node->kind() == NODE_TOPLEVEL_LIST)) {
    /* infer child 0 based on its kind */
    if (node->child(0)->kind() == NODE_FUNCTION) {
      inferFunction(node->child(0));
    } else if (node->child(0)->kind() == NODE_CONST) {
      inferGlobal(node->child(0), true);
    } else if (node->child(0)->kind() == NODE_GLOBAL) {
      inferGlobal(node->child(0));
    } else if (node->child(0)->kind() == NODE_CLASS) { 
      inferClass(node->child(0));
    }

    /* recursively infer any other functions */
    inferTypes(node->child(1));
  }
}
