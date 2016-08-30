/* stuff for the lexer */

#ifndef LEXER_H
#define LEXER_H

class QFile;
class Node;

/* reset lexer internal state */
void reset_lexer();

/* set the file to lex from */
void setLexFile(QFile& file);

/* the lexer function */
int yylex();

/* this function calls yyparse on a file and returns the parse tree node */
Node* parseFile(const tstring& fname);

#endif
