/* lexer.h
 * declares functions for the lexer */

#ifndef LEXER_H
#define LEXER_H

class QFile;
class Node;

/* reset lexer internal state */
void reset_lexer();

/* set the file to lex from */
void setLexFile(QFile& file);

/* clean up*/
void deleteLexFile();

/* the lexer function */
int yylex();

/* this function generates a new unique id that can't be used by tetra code itself */
String generateNewId();

/* this function calls yyparse on a file and returns the parse tree node */
Node* parseFile(const String& fname);

#endif
