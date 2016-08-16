/* stuff for the lexer */

#ifndef LEXER_H
#define LEXER_H

/* reset lexer internal state */
void reset_lexer();

/* the lexer function */
int yylex( );

/* this function calls yyparse on a file and returns the parse tree node */
Node* parseFile(const tstring& fname);


#endif

