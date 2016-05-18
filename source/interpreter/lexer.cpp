/* lexer.cpp
 * this file provides lexical analysis for Tetra it implements the yylex
 * function used by the parser.  Tetra does not use flex principally because
 * doing whitespace-based blocking is difficult due to the limitations of flex
 * (not being able to capture and respond to EOF) */

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#include "frontend.h"
#include "parser.genh"

using namespace std;

/* whether or not we have seen whitespace so far this line */
int start_of_line = 1;

/* the number of spaces used for one indentation level */
int spaces_per_indent = 0;

/* the current number of indent levels */
int indent_level = 0;

/* the number of DEDENTs to return before anything else */
int dedents_left = 0;

/* line number we are at - used for error messages */
int yylineno = 1;

/* reset the lexer state */
void reset_lexer() {
  start_of_line = 1;
  spaces_per_indent = 0;
  indent_level = 0;
  dedents_left = 0;
  yylineno = 1;
}

/* the symbol used to comunicate with bison */
extern YYSTYPE yylval;

/* the istream to use for doing all input */
istream* in;

/* look up a string and return its token code */
int lookupId(const string& id) {
  yylval.lineno = yylineno;
  if (id == "if") {
    return TOK_IF;
  }
  if (id == "elif") {
    return TOK_ELIF;
  }
  if (id == "else") {
    return TOK_ELSE;
  }
  if (id == "for") {
    return TOK_FOR;
  }
  if (id == "in") {
    return TOK_IN;
  }
  if (id == "parallel") {
    return TOK_PARALLEL;
  }
  if (id == "while") {
    return TOK_WHILE;
  }
  if (id == "continue") {
    return TOK_CONTINUE;
  }
  if (id == "break") {
    return TOK_BREAK;
  }
  if (id == "def") {
    return TOK_DEF;
  }
  if (id == "or") {
    return TOK_OR;
  }
  if (id == "and") {
    return TOK_AND;
  }
  if (id == "not") {
    return TOK_NOT;
  }
  if (id == "pass") {
    return TOK_PASS;
  }
  if (id == "return") {
    return TOK_RETURN;
  }
  if (id == "background") {
    return TOK_BACKGROUND;
  }
  if (id == "lock") {
    return TOK_LOCK;
  }
  if (id == "int") {
    return TOK_INT;
  }
  if (id == "real") {
    return TOK_REAL;
  }
  if (id == "bool") {
    return TOK_BOOL;
  }
  if (id == "constant") {
    return TOK_CONST;
  }
  if (id == "global") {
    return TOK_GLOBAL;
  }
  if (id == "string") {
    return TOK_STRING;
  }
  if (id == "init") {
    return TOK_INIT;
  }
  if (id == "none") {
    return TOK_NONE;
  }
  if (id == "lambda") {
    return TOK_LAMBDA;
  }
  if (id == "wait") {
    return TOK_WAIT;
  }
  if (id == "self") {
    return TOK_SELF;
  }
  if (id == "class") {
    return TOK_CLASS;
  }
  if (id == "open") {
    return TOK_OPEN;
  }
  if (id == "import") {
    return TOK_IMPORT;
  }
  if (id == "mutex") {
    return TOK_MUTEX;
  }
  if (id == "task") {
    return TOK_TASK;
  }
  if (id == "declare") {
    return TOK_DECLARE;
  }
  if (id == "true") {
    yylval.boolval = true;
    return TOK_BOOLVAL;
  } else if (id == "false") {
    yylval.boolval = false;
    return TOK_BOOLVAL;
  }

  /* save the identifier */
  strcpy(yylval.stringval, id.c_str());
  return TOK_IDENTIFIER;
}

/* lex an identifier (or reserved word) given a start */
int lexIdent(int start) {
  string id;
  id.push_back((char)start);

  while (isalnum(in->peek()) || in->peek() == '_') {
    id.push_back(in->get());
  }

  return lookupId(id);
}

/* lex a number
 * TODO handle more bases, scientific notation etc. */
int lexNumber(int start) {
  string number;
  number.push_back((char)start);

  /* have we seen a decimal point yet? */
  bool seendot = (start == '.');

  /* scan forward */
  while (true) {
    char next = in->peek();

    if (isdigit(next)) {
      number.push_back(next);
      in->get();
      continue;
    }

    if (next == '.' && seendot) {
      break;
    }

    if (next == '.') {
      /* hesitantly read the . */
      next = in->get();

      /* check if next is dot too */
      if (in->peek() == '.') {
        /* put the . back and bail */
        in->putback(next);
        break;
      } else {
        number.push_back(next);
        seendot = true;
        continue;
      }
    }

    /* it is not a number or dot, just break! */
    break;
  }

  /* if there's no decimal its an int */
  if (number.find('.') == string::npos) {
    yylval.intval = atoi(number.c_str());
    return TOK_INTVAL;
  } else {
    yylval.realval = atof(number.c_str());
    return TOK_REALVAL;
  }
}

/* lex a string constant */
int lexString() {
  string str;
  while (true) {
    char next = in->get();
    if (next == '\\') {
      /* get the thing after this */
      next = in->get();
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

  /* save the string */
  strcpy(yylval.stringval, str.c_str());
  return TOK_STRINGVAL;
}

/* the lexer function - Tetra would have used flex, but it is not possible
 * (without
 * horrible hacks) to detect EOF and add DEDENT tokens - which makes it tough to
 * do
 * whitespace based blocking a la Python */
int yylex() {
  /* if there are dedents left to return, do it */
  if (dedents_left > 0) {
    dedents_left--;
    indent_level--;
    return TOK_DEDENT;
  }

  /* read in the next character in the stream */
  int next = in->get();

  /* check for EOF */
  if (in->eof()) {
    if (indent_level != 0) {
      dedents_left = indent_level;
      return yylex();
    } else {
      return 0;
    }
  }

  /* we do NOT allow tabs */
  if (next == '\t') {
    throw Error("Tab characters are not allowed in Tetra!", yylineno);
  }

  /* if it's a new line, set to beginning of line and return next */
  if (next == '\n') {
    start_of_line = 1;
    yylineno++;
    return TOK_NEWLINE;
  }

  /* if it's whitespace, NOT at the start of a line, ignore it */
  if (!start_of_line && next == ' ') {
    /* recurse to skip it */
    return yylex();
  }

  /* if it's whitespace and IS at the start of the line */
  if (start_of_line && next == ' ') {
    /* count the spaces */
    int spaces = 1;
    while (in->peek() == ' ') {
      in->get();
      spaces++;
    }
    start_of_line = 0;

    /* if this is the first one, treat it as the level */
    if (spaces_per_indent == 0) {
      spaces_per_indent = spaces;
    }

    /* level is spaces / spaces_per_indent */
    int level = spaces / spaces_per_indent;
    if ((spaces % spaces_per_indent) != 0) {
      throw Error("Indentation level inconsistent.", yylineno);
    }

    /* if the level is greater than the current one (by one) indent */
    if (level == (indent_level + 1)) {
      indent_level++;
      return TOK_INDENT;
    }

    /* If the level is EVEN greater than that, error */
    if (level > indent_level) {
      cerr << "Level = " << level << endl;
      cerr << "Indent level = " << indent_level << endl;
      throw Error("Too much indentation.", yylineno);
    }

    /* if the level is less than the current one */
    if (level < indent_level) {
      /* set dedents_left to the difference */
      dedents_left = indent_level - level;

      /* recurse */
      return yylex();
    }

    /* if it's the same, just move on */
    if (level == indent_level) {
      return yylex();
    }

    /* else it's the start of line, but NOT whitespace */
  } else if (start_of_line) {
    start_of_line = 0;
    /* if there is indentation, set the dedents to return to that - 1, and
     * return 1 */
    if (indent_level > 0) {
      /* put the thing we read back! */
      in->putback(next);
      dedents_left = indent_level - 1;
      indent_level--;
      return TOK_DEDENT;
    }
  }

  /* it's not start of line any more */
  start_of_line = 0;

  /* if it's a letter or underscore, we have an identifier (or reserved word) */
  if (isalpha(next)) {
    return lexIdent(next);
  }

  /* if it's three decimal points, it's an elipsis */
  if (next == '.' && in->peek() == '.') {
    in->get();

    if (in->get() != '.') {
      throw Error("Lexical error: '..' not correct", yylineno);
    }

    return TOK_ELLIPSIS;
  }

  /* if it's a number or decimal point, we have a number of some kind */
  if (isdigit(next) || (next == '.' && isdigit(in->peek()))) {
    return lexNumber(next);
  }

  /* check for the dot operator */
  if (next == '.') {
    return TOK_DOT; 
  }

  /* handle comments */
  if (next == '#') {
    /* eat it all */
    while (in->peek() != '\n') {
      in->get();
    }
    /* get the new line */
    in->get();
    start_of_line = 1;
    yylineno++;

    /* skip it */
    return yylex();
  }

  /* handle string constants */
  if (next == '"') {
    return lexString();
  }

  /* character operators and punctuation */
  yylval.lineno = yylineno;
  switch (next) {
    /* single character ones */
    case '~':
      return TOK_BITNOT;
    case '(':
      return TOK_LEFTPARENS;
    case ')':
      return TOK_RIGHTPARENS;
    case ',':
      return TOK_COMMA;
    case ';':
      return TOK_SEMICOLON;
    case ':':
      return TOK_COLON;
    case '[':
      return TOK_LEFTBRACKET;
    case ']':
      return TOK_RIGHTBRACKET;
    case '{':
      return TOK_LEFTBRACE;
    case '}':
      return TOK_RIGHTBRACE;

    /* ones that have some single and double ones */
    case '=':
      if (in->peek() == '=') {
        in->get();
        return TOK_EQ;
      } else {
        return TOK_ASSIGN;
      }
    case '+':
      if (in->peek() == '=') {
        in->get();
        return TOK_PLUSEQ;
      } else {
        return TOK_PLUS;
      }
    case '-':
      if (in->peek() == '=') {
        in->get();
        return TOK_MINUSEQ;
      } else if (in->peek() == '>') {
        in->get();
        return TOK_RIGHTARROW;
      } else {
        return TOK_MINUS;
      }
    case '*':
      if (in->peek() == '=') {
        in->get();
        return TOK_TIMESEQ;
      } else if (in->peek() == '*') {
        in->get();
        if (in->peek() == '=') {
          in->get();
          return TOK_EXPEQ;
        } else {
          return TOK_EXP;
        }
      } else {
        return TOK_TIMES;
      }
    case '/':
      if (in->peek() == '=') {
        in->get();
        return TOK_DIVIDEEQ;
      } else {
        return TOK_DIVIDE;
      }
    case '%':
      if (in->peek() == '=') {
        in->get();
        return TOK_MODULUSEQ;
      } else {
        return TOK_MODULUS;
      }
    case '^':
      if (in->peek() == '=') {
        in->get();
        return TOK_XOREQ;
      } else {
        return TOK_BITXOR;
      }
    case '|':
      if (in->peek() == '=') {
        in->get();
        return TOK_OREQ;
      } else {
        return TOK_BITOR;
      }
    case '&':
      if (in->peek() == '=') {
        in->get();
        return TOK_ANDEQ;
      } else {
        return TOK_BITAND;
      }
    case '!':
      if (in->peek() != '=') {
        throw Error("Error, invalid lexeme '!'", yylineno);
      } else {
        in->get();
        return TOK_NEQ;
      }
    case '<':
      if (in->peek() == '=') {
        in->get();
        return TOK_LTE;
      } else if (in->peek() == '<') {
        in->get();
        if (in->peek() == '=') {
          in->get();
          return TOK_LSHIFTEQ;
        } else {
          return TOK_LSHIFT;
        }
      } else {
        return TOK_LT;
      }
    case '>':
      if (in->peek() == '=') {
        in->get();
        return TOK_GTE;
      } else if (in->peek() == '>') {
        in->get();
        if (in->peek() == '=') {
          in->get();
          return TOK_RSHIFTEQ;
        } else {
          return TOK_RSHIFT;
        }
      } else {
        return TOK_GT;
      }
  }

  /* if we get down here, there must be a lexer error :( */
  throw Error(string(next, 1) + " is not a valid lexeme.", yylineno);
  return 0;
}




