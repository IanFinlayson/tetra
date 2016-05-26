/* functions for type handling and inference */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include "frontend.h"
#include "parser.genh"

extern Node* root;

/* the symbol table for storing constants and globals */
map<string, Symbol> globals;

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
      for(int i = 0; i < t->subtypes->size(); i++){
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
      /* get the param list */
      std::string typeString = "(";
      for(int i=1; i<t->subtypes->size(); i++){
        typeString += typeToString(&((*(t->subtypes))[i])) + ",";
      }
      /* if the tuple has more than one element ... */
      if(t->subtypes->size() > 2)
        /* then get rid of the trailing comma */
        typeString[typeString.size()-1] = '\0';

      /* add return type and return the whole thing*/
      return typeString += ")->" + typeToString(&((*(t->subtypes))[0]));
    }
    default:
      throw Error("typeToString: Unknown data type");
  }
}

/* data type functions */
DataType::DataType(DataTypeKind kind) {
  this->kind = kind;
  this->subtypes = new std::vector<DataType>;
}

DataType::~DataType() {
  delete(&this->kind);
  delete(&this->className);
  delete(&this->subtypes);
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
    int i = 0;
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
DataType* inferExpression(Node* expr, Node* func);

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
  if ((t->getKind() != TYPE_VECTOR) && (t->getKind() != TYPE_STRING)) {
    throw Error("len function must be called with either a string or a vector",
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

/* this do checking of types on a vector reference */
DataType* checkVector(Node* vec, Node* func) {
  /* look up the left hand side */
  Symbol sym = func->lookupSymbol(vec->child(0)->getString(), vec->getLine());

  /* count the indices on the right (also ensure they are ints! */
  int levels = countIndices(vec->child(1), func);

  /* count the dimension of a vector */
  int dim = countDimensions(sym.getType());

  /* make sure dim is at least big enough for this many indexes */
  if (dim < levels) {
    throw Error("Too many indexes on vector", vec->getLine());
  }

  /* the base type is that of the symbol  */
  DataType* result = new DataType(baseType(sym.getType()));

  /* add veector wrapper for dim-levels times */
  for (int i = 0; i < (dim - levels); i++) {
    DataType* r = new DataType(TYPE_VECTOR);
    r->subtypes->push_back(*result);
    result = r;
  }

  return result;
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
DataType* inferFuncall(Node* funcall, Node* func) {
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
DataType* inferExpressionPrime(Node* expr, Node* func) {
  if (!expr) return NULL;

  /* the left hand side, right hand side, and result used below */
  DataType *lhs, *rhs;

  /* switch on the type of expression */
  switch (expr->kind()) {
    case NODE_ASSIGN: {
                        /* get the type of the right hand side */
                        rhs = inferExpression(expr->child(1), func);

                        /* check if this symbol exists and check that the types match */
                        if (func->hasSymbol(expr->child(0)->getString())) {
                          Symbol sym = func->lookupSymbol(expr->child(0)->getString(), 0);
                          if (*sym.getType() != *rhs) {
                            throw Error(
                                "Assigning '" + expr->child(0)->getString() + "' to a new type",
                                expr->getLine());
                          }
                          /* set the node type as well */
                          expr->child(0)->setDataType(rhs);
                        } else {
                          /* it's not there, so we should insert a new one */
                          func->insertSymbol(
                              Symbol(expr->child(0)->getString(), rhs, expr->getLine()));
                          /* set the node type as well */
                          expr->child(0)->setDataType(rhs);
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
    case NODE_EQ:
    case NODE_NEQ:
                      /* check that both sides have the same type
                       * TODO ar some point add in int->real promotion */
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

    case NODE_INDREF:
                      /* check vector */
                      return checkVector(expr, func);
    case NODE_INDEX:
                      throw Error("inferExpression: should not an index here");
    case NODE_VECRANGE: {
                          /* a vecrange can only possibly be a vector of ints */
                          DataType* t = new DataType(TYPE_VECTOR);
                          t->subtypes->push_back(*new DataType(TYPE_INT));
                          return t;
                        }
    case NODE_FUNCALL:
                        return inferFuncall(expr, func);
    case NODE_ACTUAL_PARAM_LIST:
                        throw Error("inferExpression: should not a param list here");
                        break;

    case NODE_IDENTIFIER: {
                            /* check if it's a global first */
                            if (globals.count(expr->getString()) > 0) {
                              Symbol sym = globals.find(expr->getString())->second;
                              return sym.getType();
                            }

                            /* look it up and return that type */
                            Symbol sym = func->lookupSymbol(expr->getString(), expr->getLine());
                            return sym.getType();
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
                        dt->subtypes->push_back(*inferExpression(expr->child(0), func));

                        /* if there are more than one child, recurse on them too */
                        for (int i = 1; i < expr->numChildren(); i++) {
                          DataType* other = inferExpression(expr->child(i), func);

                          /* check that they match! */
                          if (*other != *dt) {
                            throw Error("Mismatched vector types", expr->getLine());
                          }
                        }

                        return dt;
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
    case NODE_BACKGROUND:
                   /* check the sub-block */
                   inferBlock(block->child(0), func);
                   break;
    case NODE_LOCK:
                   /* how will locks actually be implemented??? */
                   inferBlock(block->child(1), func);
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
void inferFunction(Node* node) {
  /* only works on functions */
  if (!node || (node->kind() != NODE_FUNCTION)) {
    return;
  }

  /* if there is a block for parameters */
  if (node->numChildren() > 1) {
    /* add the parameters into the table */
    addParams(node->child(0), node);
    inferBlock(node->child(1), node);
  } else {
    /* just infer the body of the function */
    inferBlock(node->child(0), node);
  }
}

/* infer a constant definition */
void inferConst(Node* node) {
  /* get the type of the right hand side */
  DataType* rhs = inferExpression(node->child(1), NULL);

  /* check if this symbol exists and check that the types match */
  if (globals.count(node->child(0)->getString()) > 0) {
    throw Error("Constant '" + node->child(0)->getString() +
        "' has been defined.");
  } else {
    /* add it in */
    globals.insert(pair<string, Symbol>(
          node->child(0)->getString(),
          Symbol(node->child(0)->getString(), rhs, node->getLine())));

    /* set the node type as well */
    node->child(0)->setDataType(rhs);
  }
}

/* infer a global definition */
void inferGlobal(Node* node) {
  /* get the type of the right hand side */
  DataType* rhs = inferExpression(node->child(1), NULL);

  /* check if this symbol exists and check that the types match */
  if (globals.count(node->child(0)->getString()) > 0) {
    throw Error("Global '" + node->child(0)->getString() +
        "' has been defined.");
  } else {
    /* add it in */
    globals.insert(pair<string, Symbol>(
          node->child(0)->getString(),
          Symbol(node->child(0)->getString(), rhs, node->getLine())));

    /* set the node type as well */
    node->child(0)->setDataType(rhs);
  }
}

/* this function does type checking/type inference on a parse tree */
void inferTypes(Node* node) {
  /* infer each function */
  if (node && (node->kind() == NODE_TOPLEVEL_LIST)) {
    /* infer child 0 based on its kind */
    if (node->child(0)->kind() == NODE_FUNCTION) {
      inferFunction(node->child(0));
    } else if (node->child(0)->kind() == NODE_CONST) {
      inferConst(node->child(0));
    } else if (node->child(0)->kind() == NODE_GLOBAL) {
      inferGlobal(node->child(0));
    }

    /* recursively infer any other functions */
    inferTypes(node->child(1));
  }
}
