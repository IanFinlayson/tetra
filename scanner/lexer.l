%{
#include <stdio.h>
#include "tokens.h"

#define MAX_INDENTS 50
int dedents;
int indents[MAX_INDENTS];
int *num_ind = indents;
%}

%option noyywrap warn 
%x indentation strings
%%

<*>^[[:blank:]]*\n     /* skip blank lines */
<strings>{  /* TODO: make strings more robust */
    (\\\"|[^"\n])+      /* escaped or not a quote or endline, do nothing */
    \n  { printf("Error: non-terminated string.\n  Exiting.\n"); exit (1); }
    \" {
        BEGIN(INITIAL);
    }
}
<indentation>^[^[:space:]] { /* 0 whitespace to begin line */
    BEGIN(INITIAL);
    yyless(0);
    dedents = 0;
    for (; *num_ind != 0; num_ind--) {
        dedents++;
    }
    return TOK_DEDENT;
}
<indentation>^[[:blank:]]* {   
    BEGIN(INITIAL);
    /* no indentation change */
    if (yyleng == *num_ind) {
        return TOK_NULL; 
    }
    /* indentation increased */
    if (yyleng > *num_ind) { 
        num_ind++;
        if ((num_ind - indents) >= MAX_INDENTS) {
            fprintf(stderr, "Too many levels of indentation.\n");
            fprintf(stderr, "Exiting lexical analysis.\n");
            exit(1);
        }
        *num_ind = yyleng;
        return TOK_INDENT;
    }
    /* indentation decreased */ 
    dedents = 0;
    for (; *num_ind != yyleng; num_ind--) {
        dedents++;
        if (*num_ind == 0) {
            fprintf(stderr, "Dedented to non-matching column.\n");
            fprintf(stderr, "Exiting.\n");
            exit(1);
        }
    }
    return TOK_DEDENT;
}
\n  {
    BEGIN(indentation);
    return TOK_NEWLINE;
}
\" {
    BEGIN(strings);
    return TOK_STRING;
}
[[:blank:]]+                        /* skip spaces & tabs */
#.*$                                /* comments */
true                                |
false                               { return TOK_BOOL; }
[[:digit:]]+                        |
0[Bb][01]+                          |
0[Xx][[:xdigit:]]+	                { return TOK_INT; }
[[:digit:]]+f                       |
[[:digit:]]+"."[[:digit:]]*         |
"."[[:digit:]]+	                    { return TOK_REAL; }
    /* \"(\\\"|[^"])*\"	                { return TOK_STRING; } */
if	                                { return TOK_IF; }
else	                            { return TOK_ELSE; }
for	                                { return TOK_FOR; }
forall	                            { return TOK_FORALL; }
while	                            { return TOK_WHILE; }
def	                                { return TOK_DEF; }
"<"	                                { return TOK_LT; }
">"	                                { return TOK_GT; }
"<="	                            { return TOK_LTE; }
">="	                            { return TOK_GTE; }
"=="	                            { return TOK_EQ; }
"!="	                            { return TOK_NEQ; }
"+"	                                { return TOK_PLUS; }
"-"	                                { return TOK_MINUS; }
"/"	                                { return TOK_DIV; }
"*"	                                { return TOK_MULT; }
"**"	                            { return TOK_EXP; }
"/="	                            { return TOK_DIVEQ; }
"*="	                            { return TOK_MULTEQ; }
"+="	                            { return TOK_PLUSEQ; }
"-="	                            { return TOK_MINUSEQ; }
"&&"	                            { return TOK_AND; }
"||"	                            { return TOK_OR; }
[[:alpha:]_][[:alnum:]_]*	        { return TOK_IDENT; }

%%

int main(int argc, char **argv)
{
    int token, i;
    indents[0] = 0;

    if (argc < 2) {
        yyin = stdin;
    } else {
        if ((yyin = fopen(argv[1], "r")) == NULL) {
            perror("Error opening file");
            exit(1);
        }
    }

    while (token = yylex()) {
        if (token == TOK_NULL) {
            continue;
        }
        if (token == TOK_DEDENT) {
            for (i = 0; i < dedents; i++) {
                printf("%d\n", token);
            }

        } else {
            printf("%d\n", token);
        }
    }

    return 0;
}
