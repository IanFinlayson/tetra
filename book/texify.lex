%{
#include <stdio.h>
#include <string.h>

/* the number of points to indent for each space found */
#define INDENT 6

void wrap_color(char* text, char* color) {
    printf("\\textcolor{%s}{%s}", color, text);
}

/* whether we have a pending new line */
int pending_newline = 0;

/* set a pending newline */
void newl_pending() {
    pending_newline = 1;
}

/* line numbering */
static int line_number = 1;
void number() {
    printf("\\textcolor{linenocolor}{%d} ", line_number++);
}


/* check if there is a pending new line and print it if need be */
void c() {
    if (pending_newline) {
        printf("\\\\\n");
        number();
        pending_newline = 0;
    }
}

%}

%option noyywrap

%%

"class" |
"def" |
"if" |
"elif" |
"else" |
"while" |
"for" |
"in" |
"parallel" |
"return" |
"open" |
"import" |
"lambda" |
"background" |
"wait" |
"lock" |
"var" |
"init" |
"self" |
"constant" |
"none" {c(); wrap_color(yytext, "keywordcolor");}

"int" |
"real" |
"string" |
"bool" |
"task" |
"mutex" {c(); wrap_color(yytext, "typecolor");}

"print" |
"len" |
"read_int" |
"read_string" |
"read_bool" |
"read_real" |
"array" |
"keys" {c(); wrap_color(yytext, "builtincolor");}

^[ ]+  {c(); 
    int spaces = strlen(yytext);    
    printf("\\hspace*{%dpt}", INDENT * spaces);
}

[0-9]+ {c(); wrap_color(yytext, "valuecolor");}
\"(\\.|[^"])*\" {c(); wrap_color(yytext, "valuecolor");}

"#".*$ {c(); 
    /* put a \ before the # sign */    
    char* stuff = malloc(strlen(yytext) + 2);
    strcpy(stuff + 1, yytext);
    stuff[0] = '\\';
    wrap_color(stuff, "commentcolor");
}

"\n" {newl_pending();}

[a-zA-Z]+ {c(); printf("%s", yytext);}
.    {c(); printf("%s", yytext);}

%%

int main() {
    /* redefine figure name to be listing */
    printf("\\renewcommand{\\figurename}{Listing}\n");

    /* preamble */
    printf("\\begin{figure}[H]\n");
    printf("\\caption{%s}\n", "\\codecaption{}");
    printf("\\label{\\codelabel{}}\n");
    
    /* hrule */
    printf("\\noindent\\makebox[\\textwidth]{\\rule{\\textwidth}{0.4pt}}\n");

    /* use teletype */
    printf("{\\ttfamily\n");

    /* turn off normal indentation */
    printf("\\setlength{\\parindent}{0pt}\\setlength{\\parskip}{0pt}\n");

    /* put in the first line number */
    number();

    /* process all input */
    while (yylex());

    /* hrule */
    printf("\\vspace{-6pt}\\\\");
    printf("\\noindent\\makebox[\\textwidth]{\\rule{\\textwidth}{0.4pt}}\n");

    /* postamble */
    printf("}\n");
    printf("\\end{figure}\n");

    /* redefine figure name back to figure */
    printf("\\renewcommand{\\figurename}{Figure}\n");
    return 0;
}

