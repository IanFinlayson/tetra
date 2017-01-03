%{
#include <stdio.h>
#include <string.h>

/* the number of points to indent for each space found */
#define INDENT 6

void wrap_color(char* text, char* color) {
    printf("\\textcolor{%s}{%s}", color, text);
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
"none" {wrap_color(yytext, "keywordcolor");}

"int" |
"real" |
"string" |
"bool" |
"task" |
"mutex" {wrap_color(yytext, "typecolor");}

"print" |
"len" |
"read_int" |
"read_string" |
"read_bool" |
"read_real" |
"array" |
"keys" {wrap_color(yytext, "builtincolor");}

^[ ]+  {
    int spaces = strlen(yytext);    
    printf("\\hspace*{%dpt}", INDENT * spaces);
}

[0-9]+ {wrap_color(yytext, "valuecolor");}
\"(\\.|[^"])*\" {wrap_color(yytext, "valuecolor");}

"#".*$ {
    /* put a \ before the # sign */    
    char* stuff = malloc(strlen(yytext) + 2);
    strcpy(stuff + 1, yytext);
    stuff[0] = '\\';
    wrap_color(stuff, "commentcolor");
}

"\n" {printf("\\\\\n");}

[a-zA-Z]+ {printf("%s", yytext);}
.    {printf("%s", yytext);}

%%

int main(int argc, char** argv) {
    /* preamble */
    char* title = strstr(argv[1], "/") + 1;
    printf("\\vspace*{-24pt}\n");
    printf("\\begin{figure}[h]\n");
    printf("\\caption{}\n");
    printf("\\label{\\codelabel{}}\n");
    printf("\\begin{tcolorbox}[title=Listing \\ref{\\codelabel{}} %s]\n", title);
    printf("{\\ttfamily\n");
    printf("\\setlength{\\parindent}{0pt}\\setlength{\\parskip}{0pt}\n");

    /* process all input */
    while (yylex());

    /* postamble */
    printf("}\n");
    printf("\\end{tcolorbox}\n");
    printf("\\end{figure}\n");
    return 0;
}

