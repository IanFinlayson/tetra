#include <iostream>
#include <string>

#include "tetra.hpp"
#include "parser.gen.hpp"

using std::cin;

/* look up a string and return its token code */
int lookupId(const std::string& id) {
  if (id == "if")       {return TOK_IF;}
  if (id == "elif")     {return TOK_ELIF;}
  if (id == "else")     {return TOK_ELSE;}
  if (id == "for")      {return TOK_FOR;}
  if (id == "in")       {return TOK_IN;}
  if (id == "forall")   {return TOK_FORALL;}
  if (id == "parallel") {return TOK_PARALLEL;}
  if (id == "while")    {return TOK_WHILE;}
  if (id == "continue") {return TOK_CONTINUE;}
  if (id == "break")    {return TOK_BREAK;}
  if (id == "def")      {return TOK_DEF;}
  if (id == "global")   {return TOK_GLOBAL;}
  if (id == "or")       {return TOK_OR;}
  if (id == "and")      {return TOK_AND;}
  if (id == "not")      {return TOK_NOT;}
  if (id == "pass")     {return TOK_PASS;}
  if (id == "return")   {return TOK_RETURN;}
  if (id == "int")      {return TOK_INT;}
  if (id == "real")     {return TOK_REAL;}
  if (id == "bool")     {return TOK_BOOL;}
  if (id == "string")   {return TOK_STRING;}
  return TOK_IDENTIFIER;
}

/* lex an identifier (or reserved word) given a start */
int lexIdent(int start) {
  std::string id((char) start, 1);

  while (isalnum(cin.peek( )) || cin.peek( ) == '_') {
    id.push_back(cin.get( ));
  }

  return lookupId(id);
}

/* lex a number */
int lexNumber(int start) {
  /* TODO */



}

/* the lexer function - Tetra would have used flex, but it is not possible (without
 * horrible hacks) to detect EOF and add DEDENT tokens - which makes it tough to do
 * whitespace based blocking a la Python */
int yylex( ) {
  /* read in the next character in the stream */
  int next = cin.get( );

  /* if it's a letter or underscore, we have an identifier (or reserved word) */
  if (isalpha(next)) {
    return lexIdent(next);
  }

  /* if it's a number or decimal point, we have a number of some kind */
  if (isdigit(next)) {
    return lexNumber(next);
  }

  /* TODO handle whitespace */

  /* TODO handle comments */

  /* TODO handle string constants */

  /* character operators and punctuation */
  switch (next) {
    /* single character ones */
    case '~': return TOK_BITNOT;
    case '(': return TOK_LEFTPARENS;
    case ')': return TOK_RIGHTPARENS;
    case ',': return TOK_COMMA;
    case ';': return TOK_SEMICOLON;
    case ':': return TOK_COLON;
    /* ones that have some single and double ones */
    case '=':
      if (cin.peek( ) == '=') {
        cin.get( );
        return TOK_EQ;
      } else {
        return TOK_ASSIGN;
      }
    case '+':
      if (cin.peek( ) == '=') {
        cin.get( );
        return TOK_PLUSEQ;
      } else {
        return TOK_PLUS;
      }
    case '-':
      if (cin.peek( ) == '=') {
        cin.get( );
        return TOK_MINUSEQ;
      } else {
        return TOK_MINUS;
      }
    case '*':
      if (cin.peek( ) == '=') {
        cin.get( );
        return TOK_TIMESEQ;
      } else if (cin.peek( ) == '*') {
        cin.get( );
        if (cin.peek( ) == '=') {
          cin.get( );
          return TOK_EXPEQ;
        } else {
          return TOK_EXP;
        }
      } else {
        return TOK_TIMES;
      }
    case '/':
      if (cin.peek( ) == '=') {
        cin.get( );
        return TOK_DIVIDEEQ;
      } else {
        return TOK_DIVIDE;
      }
    case '%':
      if (cin.peek( ) == '=') {
        cin.get( );
        return TOK_MODULUSEQ;
      } else {
        return TOK_MODULUS;
      }
    case '^':
      if (cin.peek( ) == '=') {
        cin.get( );
        return TOK_BITXOR;
      } else {
        return TOK_XOREQ;
      }
    case '|':
      if (cin.peek( ) == '=') {
        cin.get( );
        return TOK_BITOR;
      } else {
        return TOK_OREQ;
      }
    case '&':
      if (cin.peek( ) == '=') {
        cin.get( );
        return TOK_BITAND;
      } else {
        return TOK_ANDEQ;
      }
    case '!':
      if (cin.peek( ) != '=') {
        fail("Error, invalid lexeme '!'");
      } else {
        cin.get( );
        return TOK_NEQ;
      }
    case '<':
      if (cin.peek( ) == '=') {
        cin.get( );
        return TOK_LTE;
      } else if (cin.get( ) == '<') {
        cin.get( );
        if (cin.peek( ) == '=') {
          cin.get( );
          return TOK_LSHIFTEQ;
        } else {
          return TOK_LSHIFT;
        }
      } else {
        return TOK_LT;
      }
    case '>':
      if (cin.peek( ) == '=') {
        cin.get( );
        return TOK_GTE;
      } else if (cin.get( ) == '>') {
        cin.get( );
        if (cin.peek( ) == '=') {
          cin.get( );
          return TOK_RSHIFTEQ;
        } else {
          return TOK_RSHIFT;
        }
      } else {
        return TOK_GT;
      }
  }

  /* if we get down here, there must be a lexer error :( */
  fail(std::string(next, 1) + " is not a valid lexeme.");
}







