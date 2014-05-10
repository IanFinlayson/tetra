#include <iostream>
#include <string>
#include <cstdlib>

#include "tetra.hpp"
#include "parser.gen.hpp"

using std::cin;


/* whether or not we have seen whitespace so far this line */
int start_of_line = 1;





void REPLfail(std::string m) {
  std::cout << m << std::endl;
  exit(1);
}

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

  /* TODO, save the identifier */
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

/* lex a number
 * TODO handle more bases, scientific notation etc. */
int lexNumber(int start) {
  std::string number((char) start, 1);

  while (isdigit(cin.peek( )) || cin.peek( ) == '.') {
    number.push_back(cin.get( ));
  }

  /* if there's no decimal
   * TODO save the value! */
  if (number.find('.') == std::string::npos) {
    return TOK_INTVAL;
  } else {
    return TOK_REALVAL;
  }
}

/* lex a string constant */
int lexString( ) {
  std::string str;
  while (true) {
    char next = cin.get( );
    if (next == '\\') {
      /* get the thing after this */
      next = cin.get( );
      switch (next) {
        case 'n':
          str.push_back('\n');
          break;
        case 't':
          str.push_back('\t');
          break;
        case '"':
          str.push_back('"');
        case '\\':
          str.push_back('\\');
          break;
        default:
          str.push_back(next);
      }
    } else if (next == '"') {
      break;
    } else {
      str.push_back(next);
    }
  }

  /* TODO save the string */
  return TOK_STRINGVAL;
}

/* the lexer function - Tetra would have used flex, but it is not possible (without
 * horrible hacks) to detect EOF and add DEDENT tokens - which makes it tough to do
 * whitespace based blocking a la Python */
int yylex( ) {
  /* read in the next character in the stream */
  int next = cin.get( );

  /* check for EOF */
  if (cin.eof( )) {
    return 0;
  }

  /* if it's a new line, set to beginning of line and return it */
  if (next == '\n') {
    start_of_line = 1;
    return TOK_NEWLINE;
  }

  /* if it's whitespace, NOT at the start of a line, ignore it */
  if (!start_of_line && (next == ' ' || next == '\t')) {
    /* recurse to skip it */
    return yylex( );
  }

  /* if it's whitespace and IS at the start of the line */
  if (start_of_line && (next == ' ' || next == '\t')) {
    /* TODO */

  } else if (start_of_line) {
    /* else it's the start of line, but NOT whitespace TODO */

  }



  /* if it's a letter or underscore, we have an identifier (or reserved word) */
  if (isalpha(next)) {
    return lexIdent(next);
  }

  /* if it's a number or decimal point, we have a number of some kind */
  if (isdigit(next) || next == '.') {
    return lexNumber(next);
  }

  /* handle comments */
  if (next == '#') {
    /* eat it all */
    while (cin.peek( ) != '\n') {
      cin.get( );
    }
    /* get the new line */
    cin.get( );
    /* return the new line */
    return TOK_NEWLINE;
  }

  /* handle string constants */
  if (next == '"') {
    return lexString( );
  }

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
        REPLfail("Error, invalid lexeme '!'");
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
  REPLfail(std::string(next, 1) + " is not a valid lexeme.");
  return 0;
}


int main( ) {
  int token;

  do {
    std::cout << (token = yylex( )) << std::endl;
  } while (token);

  return 0;
}





