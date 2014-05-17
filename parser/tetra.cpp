/* functions for dealing with node creation, management etc. */

#include <string>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include "tetra.hpp"
#include "parser.gen.hpp"

extern Node* root;
extern int yylineno;

string itoa(int num) {
  stringstream ss;
  ss << num;
  return ss.str( );
}

/* insert a symbol into the symtable */
void insertSymbol(Symbol sym, map<string, Symbol>* symtable) {
  /* check if it's there first */
  if (symtable->count(sym.name) > 0) {
    throw Error("'" + sym.name + "' has already been declared", sym.lineno);
  }

  /* add it in */
  symtable->insert(pair<string, Symbol>(sym.name, sym));
}

/* lookup a symbol from a symbol table */
Symbol lookupSymbol(string name, map<string, Symbol>* symtable, int lineno) {
  map<string, Symbol>::iterator it = symtable->find(name);

  if (it == symtable->end( )) {
    throw Error("Symbol '" + name + "' not found!", lineno);
  }

  /* return the record */
  return it->second;
}


string typeToString(DataType* t) {
  switch (t->kind) {
    case TYPE_INT: return "int";
    case TYPE_REAL: return "real";
    case TYPE_STRING: return "string";
    case TYPE_BOOL: return "bool";
    case TYPE_VOID: return "void";
    case TYPE_VECTOR:
      return "[" + typeToString(t->subtype) + "]";
    default: throw Error("typeToString: Unknown data type");
  }
}

/* node member functions */
Node::Node(NodeType node_type) {
  this->node_type = node_type;
  this->data_type = NULL;
  stringval = "";
  intval = 0;
  realval = 0.0;
  boolval = false;
  lineno = yylineno;  /* this is often inaccurate! */
}
 
void Node::addChild(Node* child) {
  if (child) {
    children.push_back(child);
  }
}

void Node::setDataType(DataType* data_type) {
  this->data_type = data_type;
}

void Node::setStringval(TetraString stringval) {
  this->stringval = stringval;
}

void Node::setIntval(TetraInt intval) {
  this->intval = intval;
}

void Node::setBoolval(TetraBool boolval) {
  this->boolval = boolval;
}

void Node::setRealval(TetraReal realval) {
  this->realval = realval;
}

void Node::setLine(int lineno) {
  this->lineno = lineno;
}

/* this function search and replaces a string in place */
void replace(string& str, const string& from, const string& to) {
  size_t start_pos = str.find(from);
  if(start_pos != string::npos) {
    str.replace(start_pos, from.length( ), to);
    replace(str, from, to);
  }
}

/* this function takes a string and modifies it in place
 * with TOK_ missing and some other pretty printing */
void prettyPrintMessage(string& str) {
  replace(str, "PLUS", "'+'");
  replace(str, "MINUS", "'-'");
  replace(str, "TIMES", "'*'");
  replace(str, "DIVIDE", "'/'");
  replace(str, "MODULUS", "'%'");
  replace(str, "BITXOR", "'^'");
  replace(str, "BITAND", "'&'");
  replace(str, "BITOR", "'|'");
  replace(str, "BITNOT", "'~'");
  replace(str, "LEFTPARENS", "'('");
  replace(str, "RIGHTPARENS", "')'");
  replace(str, "COMMA", "','");
  replace(str, "SEMICOLON", "';'");
  replace(str, "COLON", "':'");
  replace(str, "LSHIFT", "'<<'");
  replace(str, "RSHIFT", "'>>'");
  replace(str, "EXP", "'**'");
  replace(str, "LTE", "'<='");
  replace(str, "GTE", "'>='");
  replace(str, "EQ", "'=='");
  replace(str, "NEQ", "'!='");
  replace(str, "LT", "'<'");
  replace(str, "GT", "'>'");
  replace(str, "MINUSEQ", "'-='");
  replace(str, "PLUSEQ", "'+='");
  replace(str, "TIMESEQ", "'*='");
  replace(str, "DIVIDEEQ", "'/='");
  replace(str, "MODULUSEQ", "'%='");
  replace(str, "EXPEQ", "'**='");
  replace(str, "RSHIFTEQ", "'>>='");
  replace(str, "LSHIFTEQ", "'<<='");
  replace(str, "ANDEQ", "'&='");
  replace(str, "XOREQ", "'^='");
  replace(str, "OREQ", "'|='");
  replace(str, "TOK_", "");
}

/* Error exception functions */
Error::Error(const string& mesg, int lineno) {
  this->mesg = mesg;
  prettyPrintMessage(this->mesg);
  this->lineno = lineno;
}

string Error::getMessage( ) const {
  return mesg;
}

int Error::getLine( ) const {
  return lineno;
}

/* print an error */
ostream& operator<<(ostream& out, const Error& error) {
  out << "Error: ";
  if (error.getLine( )) {
    out << "(line " << error.getLine( ) << ") ";
  }

  out << error.getMessage( ) << endl;
  return out;
}

/* data type functions */
DataType::DataType(DataTypeType kind) {
  this->kind = kind;
  subtype = NULL;
}

void DataType::setSubType(DataType* subtype) {
  this->subtype = subtype;
}

bool operator==(const DataType& lhs, const DataType& rhs) {
  /* if they're not the same kind, fail */
  if (lhs.kind != rhs.kind) {
    return false;
  }

  /* if they're vectors, recursively ensure the subtypes match */
  if (lhs.kind == TYPE_VECTOR) {
    return (*(lhs.subtype) == *(rhs.subtype));
  }

  /* otherwise return true */
  return true;
}

bool operator!=(const DataType& lhs, const DataType& rhs) {
  return !(lhs == rhs);
}




Symbol::Symbol(string name, DataType* type, int lineno) {
  this->name = name;
  this->type = type;
  this->lineno = lineno;
}

Symbol::Symbol( ) {
  name = "";
  type = NULL;
  lineno = 0;
}

/* add the parameters of a function into its symtable */
void addParams(Node* params, map<string, Symbol>* symtable) {
  if (!params) return;

  /* if it's more than one, recurse on both */
  if (params->node_type == NODE_FORMAL_PARAM_LIST) {
    addParams(params->children[0], symtable);
    addParams(params->children[1], symtable);

  } 
  /* else if it's just one param, handle it */
  else if (params->node_type == NODE_FORMAL_PARAM) {
    insertSymbol(Symbol(params->stringval, params->data_type, params->lineno), symtable);
  }
}

DataTypeType baseType(DataType* t) {
  if (t->kind == TYPE_VECTOR)
    return baseType(t->subtype);
  else
    return t->kind;
}


DataType* inferExpression(Node* expr, map<string, Symbol>* symtable);

int countIndices(Node* idx, map<string, Symbol>* symtable) {
  if (!idx) return 0;

  /* check the index for being an int */
  DataType* itype = inferExpression(idx->children[0], symtable);
  if (itype->kind != TYPE_INT) {
    throw Error("Index must be integral", idx->lineno);
  }

  /* return 1 for this, and recurse */
  return 1 + countIndices(idx->children[1], symtable);
}

int countDimensions(DataType* t) {
  if (t->kind == TYPE_VECTOR) {
    return 1 + countDimensions(t->subtype);
  } else {
    return 0;
  }
}

/* this is fairly annoying */
DataType* checkVector(Node* vec, map<string, Symbol>* symtable) {
  /* look up the left hand side */
  Symbol sym = lookupSymbol(vec->children[0]->stringval, symtable, vec->lineno);

  /* count the indices on the right (also ensure they are ints! */
  int levels = countIndices(vec->children[1], symtable);

  /* count the dimension of a vector */
  int dim = countDimensions(sym.type);

  /* make sure dim is at least big enough for this many indexes */
  if (dim < levels) {
    throw Error("Too many indexes on vector", vec->lineno);
  }

  /* the base type is that of the symbol  */
  DataType* result = new DataType(baseType(sym.type));

  /* add veector wrapper for dim-levels times */
  for (int i = 0; i < (dim - levels); i++) {
    DataType* r = new DataType(TYPE_VECTOR);
    r->setSubType(result);
    result = r;
  }

  return result;
}

Node* findFunction(const string& name, int lineno) {
  Node* f = root;

  while (f && (f->children.size( ) > 0)) {
    /* see if this node is the thing */
    if (f->children[0]->stringval == name) {
      return f->children[0];
    }

    /* move to the next */
    f = f->children[1];
  }

  throw Error("Function '" + name + "' not defined", lineno);
}


/* infer the function of a function call and check the types */
DataType* inferFuncall(Node* funcall, map<string, Symbol>* symtable) {

  /* find the function node this thing matches */
  Node* f = findFunction(funcall->stringval, funcall->lineno);

  /* for each param, and also for each expression, make sure they match up */
  Node* formal = f->children[0];
  Node* actual = funcall->children[0];

  int p = 1;
  while (formal && actual) {
    DataType* t;
    
    /* it could be a list, or an actual param */
    if (actual->node_type == NODE_ACTUAL_PARAM_LIST) 
      t = inferExpression(actual->children[0], symtable);
    else
      t = inferExpression(actual, symtable);

    if (*t != *(formal->data_type)) {
      throw Error("Function '" + funcall->stringval + "' expected " + 
          typeToString(formal->data_type) + " but was called with " + typeToString(t) + 
          " for parameter " + itoa(p), funcall->lineno);
    }
    p++;

    /* move on to the next one */
    if (formal->node_type == NODE_FORMAL_PARAM_LIST) {
     formal = formal->children[1];
    } else {
      formal = NULL;
    }

    /* move on to the next one */
    if (actual->node_type == NODE_ACTUAL_PARAM_LIST) {
      actual = actual->children[1];
    } else {
      actual = NULL;
    }
  }

  /* if there are any left, the arity mismatched */
  if (formal) {
    throw Error("Too few parameters to function '" + funcall->stringval + "'", funcall->lineno);
  } else if (actual) {
    throw Error("Too many parameters to function '" + funcall->stringval + "'", funcall->lineno);
  }

  /* return the type of function itself */
  return f->data_type;
}

/* infer the types of an expression, and also return the type */
DataType* inferExpressionPrime(Node* expr, map<string, Symbol>* symtable) {
  if (!expr) return NULL;

  /* the left hand side, right hand side, and result used below */
  DataType *lhs, *rhs;

  /* switch on the type of expression */
  switch (expr->node_type) {
    case NODE_ASSIGN: {
      /* get the type of the right hand side */
      rhs = inferExpression(expr->children[1], symtable);

      /* check if this symbol exists and check that the types match */
      if (symtable->count(expr->children[0]->stringval) > 0) {
        Symbol sym = lookupSymbol(expr->children[0]->stringval, symtable, 0);
        if (*sym.type != *rhs) {
          throw Error("Assigning '" + expr->children[0]->stringval + "' to a new type", expr->lineno);
        }
      } else {
        /* it's not there, so we should insert a new one */
        insertSymbol(Symbol(expr->children[0]->stringval, rhs, expr->lineno), symtable);
      }

      /* return the type of the rhs */
      return rhs;
      break;}

    case NODE_OR:
    case NODE_AND:
      /* check that both children are bools */
      lhs = inferExpression(expr->children[0], symtable);
      rhs = inferExpression(expr->children[1], symtable);
      if ((lhs->kind != TYPE_BOOL) || (rhs->kind != TYPE_BOOL)) {
        throw Error("Only bool values may be used with and/or", expr->lineno);
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
      lhs = inferExpression(expr->children[0], symtable);
      rhs = inferExpression(expr->children[1], symtable);
      if (*lhs != *rhs) {
        cout << "A = " << typeToString(lhs) << endl;
        cout << "B = " << typeToString(rhs) << endl;
        throw Error("Only matching types can be compared", expr->lineno);
      }

      /* the result is a bool */
      return new DataType(TYPE_BOOL);

    case NODE_NOT:
      /* check that the operand is bool */
      lhs = inferExpression(expr->children[0], symtable);
      if (lhs->kind != TYPE_BOOL) {
        throw Error("Operand of not must be a bool", expr->lineno);
      }
      return new DataType(TYPE_BOOL);

    case NODE_BITXOR:
    case NODE_BITAND:
    case NODE_BITOR:
    case NODE_SHIFTL:
    case NODE_SHIFTR:
      /* check that both operands are integers */
      lhs = inferExpression(expr->children[0], symtable);
      rhs = inferExpression(expr->children[1], symtable);

      if ((lhs->kind != TYPE_INT) || (rhs->kind != TYPE_INT)) {
        throw Error("Operands to bitwise operator must be integer", expr->lineno);
      }

      /* returns an integer back */
      return new DataType(TYPE_INT);

    case NODE_BITNOT:
      /* check that the operand is an int */
      lhs = inferExpression(expr->children[0], symtable);
      if (lhs->kind != TYPE_INT) {
        throw Error("Operand to bitwise not must be an integer", expr->lineno);
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
      lhs = inferExpression(expr->children[0], symtable);
      rhs = inferExpression(expr->children[1], symtable);

      if (*lhs != *rhs) {
        throw Error("In binary operator, the types " + typeToString(lhs) + " and " +
            typeToString(rhs) + " are not compatible", expr->lineno);
      }
      if ((lhs->kind != TYPE_INT) && (rhs->kind != TYPE_REAL)) {
        throw Error("Numeric type required", expr->lineno);
      }

      /* return the same type back */
      return new DataType(lhs->kind);

    case NODE_VECREF:
      /* check vector */
      return checkVector(expr, symtable);
    case NODE_INDEX:
      throw Error("inferExpression: should not an index here");


    case NODE_FUNCALL:
      return inferFuncall(expr, symtable);
    case NODE_ACTUAL_PARAM_LIST:
      throw Error("inferExpression: should not a param list here");
      break;

    case NODE_IDENTIFIER: {
      /* look it up and return that type */
      Symbol sym = lookupSymbol(expr->stringval, symtable, expr->lineno);
      return sym.type;
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
DataType* inferExpression(Node* expr, map<string, Symbol>* symtable) {
  /* do the inference */
  DataType* t = inferExpressionPrime(expr, symtable);

  /* assign it into this node */
  expr->data_type = t;

  /* return it */
  return t;
}

/* infer types in a block and add them into the symbol table */
void inferBlock(Node* block, map<string, Symbol>* symtable) {
  if (!block) return;

  /* switch on the different types */
  switch (block->node_type) {
    case NODE_STATEMENT:
      /* handle both children */
      inferBlock(block->children[0], symtable);
      inferBlock(block->children[1], symtable);
      break;
    case NODE_RETURN:
      /* infer the expression TODO check that it matches the return type */
      inferExpression(block->children[0], symtable);
      break;
    case NODE_IF: {
      /* infer the type of the expression */
      DataType* cond = inferExpression(block->children[0], symtable);

      /* check that it is a BOOL */
      if (cond->kind != TYPE_BOOL) {
        throw Error("if condition must be a bool", block->lineno);
      }

      /* infer both the then and else blocks */
      inferBlock(block->children[1], symtable);
      inferBlock(block->children[2], symtable);
      break;}
    case NODE_ELIF:
      /* check the first child which is the first elf clause */
      inferBlock(block->children[0], symtable);

      /* check the second child which is the chain of the rest of the elifs */
      inferBlock(block->children[1], symtable);

      /* check the third child which is the else clause (maybe NULL) */
      inferBlock(block->children[2], symtable);
      break;
    case NODE_ELIF_CHAIN:
      /* check the clause on the left and the chain on the right */
      inferBlock(block->children[0], symtable);
      inferBlock(block->children[1], symtable);
      break;
    case NODE_ELIF_CLAUSE: {
      /* check the expression on the left */
      DataType* cond = inferExpression(block->children[0], symtable);

      /* check that it is a BOOL */
      if (cond->kind != TYPE_BOOL) {
        throw Error("elif condition must be a bool", block->lineno);
      }

      /* check the statements on the right */
      inferBlock(block->children[1], symtable);
      break;}
    case NODE_WHILE: {
      /* infer the type of the expression */
      DataType* cond = inferExpression(block->children[0], symtable);

      /* check that it is a BOOL */
      if (cond->kind != TYPE_BOOL) {
        throw Error("while condition must be a bool", block->lineno);
      }

      /* infer the body */
      inferBlock(block->children[1], symtable);
      break;}
    case NODE_PARFOR:
    case NODE_FOR: {
      /* infer the type of the expression */
      DataType* expr_type = inferExpression(block->children[1], symtable);

      /* make sure it is some type of vector */
      if (expr_type->kind != TYPE_VECTOR) {
        throw Error("for expression must have vector type", block->lineno);
      }

      /* put the identifier in the symtable */
      Symbol induction;
      induction.name = block->children[0]->stringval;
      induction.type = expr_type->subtype;
      induction.lineno = block->lineno;
      insertSymbol(induction, symtable);

      /* check the block under this */
      inferBlock(block->children[2], symtable);
      break;}
    case NODE_PARALLEL:
    case NODE_BACKGROUND:
      /* check the sub-block */
      inferBlock(block->children[0], symtable);
      break;
    case NODE_LOCK:
      /* how will locks actually be implemented??? */
      inferBlock(block->children[1], symtable);
      break;

    /* these require no work... */
    case NODE_PASS:
    case NODE_BREAK:
    case NODE_CONTINUE:
      break;

    default:
      /* handle it as an expression */
      inferExpression(block, symtable);
  }
}

/* infer types based on a single function */
void inferFunction(Node* node) {
  /* only works on functions */
  if (!node || (node->node_type != NODE_FUNCTION)) {
    return;
  }

  /* create the symbol table for this function */
  node->symtable = new map<string, Symbol>( );

  /* add the parameters into the table */
  addParams(node->children[0], node->symtable);

  /* infer the body of the function */
  inferBlock(node->children[1], node->symtable);
}

/* this function does type checking/type inference on a parse tree */
void inferTypes(Node* node) {
  /* infer each function */
  if (node && (node->node_type == NODE_FUNCTION_LIST)) {
    /* infer the function on the left */
    inferFunction(node->children[0]);

    /* recursively infer any other functions */
    inferTypes(node->children[1]);
  }
}

