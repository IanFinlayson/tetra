/* functions for type handling and inference */

#include <string>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include "frontend.hpp"
#include "parser.gen.hpp"

extern Node* root;

/* return a string of an int */
string itoa(int num) {
  stringstream ss;
  ss << num;
  return ss.str( );
}

/* return a string of a data type */
string typeToString(DataType* t) {
  switch (t->getKind( )) {
    case TYPE_INT: return "int";
    case TYPE_REAL: return "real";
    case TYPE_STRING: return "string";
    case TYPE_BOOL: return "bool";
    case TYPE_VOID: return "void";
    case TYPE_VECTOR:
      return "[" + typeToString(t->getSub( )) + "]";
    default: throw Error("typeToString: Unknown data type");
  }
}

/* data type functions */
DataType::DataType(DataTypeKind kind) {
  this->kind = kind;
  subtype = NULL;
}

DataTypeKind DataType::getKind( ) const {
  return kind;
}

DataType* DataType::getSub( ) const {
  return subtype;
}

void DataType::setSubType(DataType* subtype) {
  this->subtype = subtype;
}

/* find the base type of a data type */
DataTypeKind baseType(DataType* t) {
  if (t->getKind( ) == TYPE_VECTOR)
    return baseType(t->getSub( ));
  else
    return t->getKind( );
}


/* compare two data types for equality */
bool operator==(const DataType& lhs, const DataType& rhs) {
  /* if they're not the same kind, fail */
  if (lhs.getKind( ) != rhs.getKind( )) {
    return false;
  }

  /* if they're vectors, recursively ensure the subtypes match */
  if (lhs.getKind( ) == TYPE_VECTOR) {
    return (*(lhs.getSub( )) == *(rhs.getSub( )));
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
  if (params->kind( ) == NODE_FORMAL_PARAM_LIST) {
    addParams(params->child(0), func);
    addParams(params->child(1), func);

  } 
  /* else if it's just one param, handle it */
  else if (params->kind( ) == NODE_FORMAL_PARAM) {
    func->insertSymbol(Symbol(params->getString( ), params->type( ), params->getLine( )));
  }
}

/* forward declaration needed as these are mutually-recursive */
DataType* inferExpression(Node* expr, Node* func);

/* count the number of indices used in an index operation */
int countIndices(Node* idx, Node* func) {
  if (!idx) return 0;

  /* check the index for being an int */
  DataType* itype = inferExpression(idx->child(0), func);
  if (itype->getKind( ) != TYPE_INT) {
    throw Error("Index must be integral", idx->getLine( ));
  }

  /* return 1 for this, and recurse */
  return 1 + countIndices(idx->child(1), func);
}

/* count the number of dimensions in a data type */
int countDimensions(DataType* t) {
  if (t->getKind( ) == TYPE_VECTOR) {
    return 1 + countDimensions(t->getSub( ));
  } else {
    return 0;
  }
}

/* this do checking of types on a vector reference */
DataType* checkVector(Node* vec, Node* func) {
  /* look up the left hand side */
  Symbol sym = func->lookupSymbol(vec->child(0)->getString( ), vec->getLine( ));

  /* count the indices on the right (also ensure they are ints! */
  int levels = countIndices(vec->child(1), func);

  /* count the dimension of a vector */
  int dim = countDimensions(sym.getType( ));

  /* make sure dim is at least big enough for this many indexes */
  if (dim < levels) {
    throw Error("Too many indexes on vector", vec->getLine( ));
  }

  /* the base type is that of the symbol  */
  DataType* result = new DataType(baseType(sym.getType( )));

  /* add veector wrapper for dim-levels times */
  for (int i = 0; i < (dim - levels); i++) {
    DataType* r = new DataType(TYPE_VECTOR);
    r->setSubType(result);
    result = r;
  }

  return result;
}

/* find a function by name in the parse tree */
Node* findFunction(const string& name, int lineno) {
  Node* f = root;

  while (f && (f->numChildren( ) > 0)) {
    /* see if this node is the thing */
    if (f->child(0)->getString( ) == name) {
      return f->child(0);
    }

    /* move to the next */
    f = f->child(1);
  }

  throw Error("Function '" + name + "' not defined", lineno);
}


/* infer the function of a function call and check the types */
DataType* inferFuncall(Node* funcall, Node* func) {

  /* find the function node this thing matches */
  Node* f = findFunction(funcall->getString( ), funcall->getLine( ));

  /* for each param, and also for each expression, make sure they match up */
  Node* formal = f->child(0);
  Node* actual = funcall->child(0);

  int p = 1;
  while (formal && actual) {
    DataType* t;
    
    /* it could be a list, or an actual param */
    if (actual->kind( ) == NODE_ACTUAL_PARAM_LIST) 
      t = inferExpression(actual->child(0), func);
    else
      t = inferExpression(actual, func);

    if (*t != *(formal->type( ))) {
      throw Error("Function '" + funcall->getString( ) + "' expected " + 
          typeToString(formal->type( )) + " but was called with " + typeToString(t) + 
          " for parameter " + itoa(p), funcall->getLine( ));
    }
    p++;

    /* move on to the next one */
    if (formal->kind( ) == NODE_FORMAL_PARAM_LIST) {
     formal = formal->child(1);
    } else {
      formal = NULL;
    }

    /* move on to the next one */
    if (actual->kind( ) == NODE_ACTUAL_PARAM_LIST) {
      actual = actual->child(1);
    } else {
      actual = NULL;
    }
  }

  /* if there are any left, the arity mismatched */
  if (formal) {
    throw Error("Too few parameters to function '" + funcall->getString( ) + "'", funcall->getLine( ));
  } else if (actual) {
    throw Error("Too many parameters to function '" + funcall->getString( ) + "'", funcall->getLine( ));
  }

  /* return the type of function itself */
  return f->type( );
}

/* infer the types of an expression, and also return the type */
DataType* inferExpressionPrime(Node* expr, Node* func) {
  if (!expr) return NULL;

  /* the left hand side, right hand side, and result used below */
  DataType *lhs, *rhs;

  /* switch on the type of expression */
  switch (expr->kind( )) {
    case NODE_ASSIGN: {
      /* get the type of the right hand side */
      rhs = inferExpression(expr->child(1), func);

      /* check if this symbol exists and check that the types match */
      if (func->hasSymbol(expr->child(0)->getString( ))) {
        Symbol sym = func->lookupSymbol(expr->child(0)->getString( ), 0);
        if (*sym.getType( ) != *rhs) {
          throw Error("Assigning '" + expr->child(0)->getString( ) + "' to a new type", expr->getLine( ));
        }
      } else {
        /* it's not there, so we should insert a new one */
        func->insertSymbol(Symbol(expr->child(0)->getString( ), rhs, expr->getLine( )));
      }

      /* return the type of the rhs */
      return rhs;
      break;}

    case NODE_OR:
    case NODE_AND:
      /* check that both children are bools */
      lhs = inferExpression(expr->child(0), func);
      rhs = inferExpression(expr->child(1), func);
      if ((lhs->getKind( ) != TYPE_BOOL) || (rhs->getKind( ) != TYPE_BOOL)) {
        throw Error("Only bool values may be used with and/or", expr->getLine( ));
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
        throw Error("Only matching types can be compared", expr->getLine( ));
      }

      /* the result is a bool */
      return new DataType(TYPE_BOOL);

    case NODE_NOT:
      /* check that the operand is bool */
      lhs = inferExpression(expr->child(0), func);
      if (lhs->getKind( ) != TYPE_BOOL) {
        throw Error("Operand of not must be a bool", expr->getLine( ));
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

      if ((lhs->getKind( ) != TYPE_INT) || (rhs->getKind( ) != TYPE_INT)) {
        throw Error("Operands to bitwise operator must be integer", expr->getLine( ));
      }

      /* returns an integer back */
      return new DataType(TYPE_INT);

    case NODE_BITNOT:
      /* check that the operand is an int */
      lhs = inferExpression(expr->child(0), func);
      if (lhs->getKind( ) != TYPE_INT) {
        throw Error("Operand to bitwise not must be an integer", expr->getLine( ));
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
        throw Error("In binary operator, the types " + typeToString(lhs) + " and " +
            typeToString(rhs) + " are not compatible", expr->getLine( ));
      }
      if ((lhs->getKind( ) != TYPE_INT) && (rhs->getKind( ) != TYPE_REAL)) {
        throw Error("Numeric type required", expr->getLine( ));
      }

      /* return the same type back */
      return new DataType(lhs->getKind( ));

    case NODE_VECREF:
      /* check vector */
      return checkVector(expr, func);
    case NODE_INDEX:
      throw Error("inferExpression: should not an index here");


    case NODE_FUNCALL:
      return inferFuncall(expr, func);
    case NODE_ACTUAL_PARAM_LIST:
      throw Error("inferExpression: should not a param list here");
      break;

    case NODE_IDENTIFIER: {
      /* look it up and return that type */
      Symbol sym = func->lookupSymbol(expr->getString( ), expr->getLine( ));
      return sym.getType( );
      break;}

    /* return these types */
    case NODE_INTVAL:
      return new DataType(TYPE_INT);
    case NODE_REALVAL:
      return new DataType(TYPE_REAL);
    case NODE_BOOLVAL:
      return new DataType(TYPE_BOOL);
    case NODE_STRINGVAL:
      return new DataType(TYPE_STRING);

    default:
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
  expr->setType(t);

  /* return it */
  return t;
}

/* infer types in a block and add them into the symbol table */
void inferBlock(Node* block, Node* func) {
  if (!block) return;

  /* switch on the different types */
  switch (block->kind( )) {
    case NODE_STATEMENT:
      /* handle both children */
      inferBlock(block->child(0), func);
      inferBlock(block->child(1), func);
      break;
    case NODE_RETURN: {
      /* infer the expression */
      DataType* ret = inferExpression(block->child(0), func);

      /* check that it matches the return type */
      if (*ret != *(func->type( ))) {
        throw Error("Return value type does not match function's declared type", block->getLine( ));
      }
      break;}
    case NODE_IF: {
      /* infer the type of the expression */
      DataType* cond = inferExpression(block->child(0), func);

      /* check that it is a BOOL */
      if (cond->getKind( ) != TYPE_BOOL) {
        throw Error("if condition must be a bool", block->getLine( ));
      }

      /* infer both the then and else blocks */
      inferBlock(block->child(1), func);
      inferBlock(block->child(2), func);
      break;}
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
      if (cond->getKind( ) != TYPE_BOOL) {
        throw Error("elif condition must be a bool", block->getLine( ));
      }

      /* check the statements on the right */
      inferBlock(block->child(1), func);
      break;}
    case NODE_WHILE: {
      /* infer the type of the expression */
      DataType* cond = inferExpression(block->child(0), func);

      /* check that it is a BOOL */
      if (cond->getKind( ) != TYPE_BOOL) {
        throw Error("while condition must be a bool", block->getLine( ));
      }

      /* infer the body */
      inferBlock(block->child(1), func);
      break;}
    case NODE_PARFOR:
    case NODE_FOR: {
      /* infer the type of the expression */
      DataType* expr_type = inferExpression(block->child(1), func);

      /* make sure it is some type of vector */
      if (expr_type->getKind( ) != TYPE_VECTOR) {
        throw Error("for expression must have vector type", block->getLine( ));
      }

      /* put the identifier in the func */
      func->insertSymbol(Symbol(block->child(0)->getString( ), expr_type->getSub( ), block->getLine( )));

      /* check the block under this */
      inferBlock(block->child(2), func);
      break;}
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
  if (!node || (node->kind( ) != NODE_FUNCTION)) {
    return;
  }

  /* add the parameters into the table */
  addParams(node->child(0), node);

  /* infer the body of the function */
  inferBlock(node->child(1), node);
}

/* this function does type checking/type inference on a parse tree */
void inferTypes(Node* node) {
  /* infer each function */
  if (node && (node->kind( ) == NODE_FUNCTION_LIST)) {
    /* infer the function on the left */
    inferFunction(node->child(0));

    /* recursively infer any other functions */
    inferTypes(node->child(1));
  }
}

