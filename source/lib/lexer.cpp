/* lexer.cpp
 * this file provides lexical analysis for Tetra it implements the yylex
 * function used by the parser.  Tetra does not use flex principally because
 * doing whitespace-based blocking is difficult due to the limitations of flex
 * (not being able to capture and respond to EOF) */

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#include <QFile>
#include <QTextStream>

#include "tetra.h"

/* whether or not we have seen whitespace so far this line */
int start_of_line = 1;

/* the number of spaces used for one indentation level */
int spaces_per_indent = 0;

/* the current number of indent levels */
int indent_level = 0;

/* the number of DEDENTs to return before anything else */
int dedents_left = 0;

/* if we need an extra new line to fake a blank line at eof */
int extra_newl = 0;

/* line number we are at - used for error messages */
int yylineNumber = 1;

/* reset the lexer state */
void reset_lexer() {
    start_of_line = 1;
    spaces_per_indent = 0;
    indent_level = 0;
    dedents_left = 0;
    yylineNumber = 1;
}

/* the symbol used to comunicate with bison */
extern YYSTYPE yylval;

/* the input stream to use for doing all input */
QTextStream* in;

/* a queue of characters which are buffered */
std::deque<QChar> buffer;

/* set the file we are reading from */
void setLexFile(QFile& file) {
    in = new QTextStream(&file);
    buffer.clear();
}

/* delete the file stream */
void deleteLexFile() {
    delete in;
}

/* peek at the next thing from the stream */
QChar peek() {
    /* if the buffer is empty, read one in first */
    if (buffer.size() == 0) {
        QChar next;
        (*in) >> next;
        buffer.push_front(next);
    }

    /* return the next thing from the buffer (keeping it) */
    return buffer.front();
}

/* get the next thing from the stream */
QChar get() {
    /* if there is a buffered thing, remove and return it */
    if (buffer.size() != 0) {
        QChar next = buffer.front();
        buffer.pop_front();
        return next;
    } else {
        /* just read one */
        QChar next;
        (*in) >> next;
        return next;
    }
}

/* put something back in the stream */
void putback(QChar c) {
    buffer.push_front(c);
}

/* check if we are out of characters */
bool eof() {
    return buffer.empty() && in->atEnd();
}

/* look up a string and return its token code */
int lookupId(const String& id) {
    yylval.lineNumber = yylineNumber;
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
    if (id == "as") {
        return TOK_AS;
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
    if (id == "true") {
        yylval.boolValue = new Bool;
        *(yylval.boolValue) = Bool(true);
        return TOK_BOOLVAL;
    } else if (id == "false") {
        yylval.boolValue = new Bool;
        *(yylval.boolValue) = Bool(false);
        return TOK_BOOLVAL;
    }

    /* save the identifier */
    yylval.stringval = new String;
    *(yylval.stringval) = id;
    return TOK_IDENTIFIER;
}

/* lex an identifier (or reserved word) given a start */
int lexIdent(QChar start) {
    String id;
    id.push_back(start);

    while (peek().isLetterOrNumber() || peek() == '_') {
        id.push_back(get());
    }

    return lookupId(id);
}

/* lex a number
 * TODO handle more bases, scientific notation etc. */
int lexNumber(QChar start) {
    String number;
    number.push_back(start);

    /* have we seen a decimal point yet? */
    bool seendot = (start == '.');

    /* scan forward */
    while (true) {
        QChar next = peek();

        if (next.isDigit()) {
            number.push_back(next);
            get();
            continue;
        }

        if (next == '.' && seendot) {
            break;
        }

        if (next == '.') {
            /* hesitantly read the . */
            next = get();

            /* check if next is dot too */
            if (peek() == '.') {
                /* put the . back and bail */
                putback(next);
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
    if (number.find('.') == -1) {
        yylval.intValue = new Int;
        *(yylval.intValue) = number.toInt();
        return TOK_INTVAL;
    } else {
        yylval.realValue = new Real;
        *(yylval.realValue) = number.toReal();
        return TOK_REALVAL;
    }
}

/* lex a string constant */
int lexString() {
    String str;
    while (true) {
        QChar next = get();
        if (next == '\\') {
            /* get the thing after this */
            next = get();
            if (next == 'n') {
                str.push_back('\n');
            } else if (next == 't') {
                str.push_back('\t');
            } else if (next == '"') {
                str.push_back('"');
            } else if (next == '\\') {
                str.push_back('\\');
            } else {
                str.push_back(next);
            }
        } else if (next == '"') {
            break;
        } else {
            str.push_back(next);
        }
    }

    /* save the string */
    yylval.stringval = new String;
    *(yylval.stringval) = str;
    return TOK_STRINGVAL;
}

/* the lexer function - Tetra would have used flex, but it is not possible
 * (without
 * horrible hacks) to detect EOF and add DEDENT tokens - which makes it tough to
 * do
 * whitespace based blocking a la Python */
int yylex() {
    /* if we need the extra new line at eof, do that first */
    if (extra_newl) {
        extra_newl = 0;
        return TOK_NEWLINE;
    }

    /* if there are dedents left to return, do it */
    if (dedents_left > 0) {
        dedents_left--;
        indent_level--;
        return TOK_DEDENT;
    }

    /* read in the next character in the stream */
    QChar next = get();

    /* handle comments */
    if (next == '#') {
        /* eat it all */
        while (peek() != '\n') {
            get();
        }
        /* get the new line */
        get();
        start_of_line = 1;
        yylineNumber++;

        /* skip it */
        return yylex();
    }

    /* check for EOF */
    if (eof()) {
        if (indent_level != 0) {
            dedents_left = indent_level;
            extra_newl = 1;
            return yylex();
        } else {
            return 0;
        }
    }

    /* we do NOT allow tabs */
    if (next == '\t') {
        throw Error("Tab characters are not allowed in Tetra!", yylineNumber);
    }

    /* if it's a new line, set to beginning of line and return next */
    if (next == '\n') {
        start_of_line = 1;
        yylineNumber++;
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
        while (peek() == ' ') {
            get();
            spaces++;
        }

        start_of_line = 0;

        /* handle the case where there is nothing but spaces */
        if (peek() == '\n') {
            get();
            start_of_line = 1;
            yylineNumber++;
            return TOK_NEWLINE;
        }

        /* handle the case where it is just a comment */
        if (peek() == '#') {
            get();
            while (peek() != '\n') {
                get();
            }
            get();
            yylineNumber++;
            start_of_line = 1;
            return yylex();
        }

        /* if this is the first one, treat it as the level */
        if (spaces_per_indent == 0) {
            spaces_per_indent = spaces;
        }

        /* level is spaces / spaces_per_indent */
        int level = spaces / spaces_per_indent;
        if ((spaces % spaces_per_indent) != 0) {
            throw Error("Indentation level inconsistent.", yylineNumber);
        }

        /* if the level is greater than the current one (by one) indent */
        if (level == (indent_level + 1)) {
            indent_level++;
            return TOK_INDENT;
        }

        /* If the level is EVEN greater than that, error */
        if (level > indent_level) {
            std::cerr << "Level = " << level << std::endl;
            std::cerr << "Indent level = " << indent_level << std::endl;
            throw Error("Too much indentation.", yylineNumber);
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
            putback(next);
            dedents_left = indent_level - 1;
            indent_level--;
            return TOK_DEDENT;
        }
    }

    /* it's not start of line any more */
    start_of_line = 0;

    /* if it's a letter or underscore, we have an identifier (or reserved word)
     */
    if (next.isLetter() || next == '_') {
        return lexIdent(next);
    }

    /* if it's three decimal points, it's an elipsis */
    if (next == '.' && peek() == '.') {
        get();

        if (get() != '.') {
            throw Error("Lexical error: '..' not correct", yylineNumber);
        }

        return TOK_ELLIPSIS;
    }

    /* if it's a number or decimal point, we have a number of some kind */
    if (next.isDigit() || (next == '.' && peek().isDigit())) {
        return lexNumber(next);
    }

    /* check for the dot operator */
    if (next == '.') {
        return TOK_DOT;
    }

    /* handle string constants */
    if (next == '"') {
        return lexString();
    }

    /* character operators and punctuation */
    yylval.lineNumber = yylineNumber;

    switch (next.unicode()) {
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
            if (peek() == '=') {
                get();
                return TOK_EQ;
            } else {
                return TOK_ASSIGN;
            }
        case '+':
            if (peek() == '=') {
                get();
                return TOK_PLUSEQ;
            } else {
                return TOK_PLUS;
            }
        case '-':
            if (peek() == '=') {
                get();
                return TOK_MINUSEQ;
            } else if (peek() == '>') {
                get();
                return TOK_RIGHTARROW;
            } else {
                return TOK_MINUS;
            }
        case '*':
            if (peek() == '=') {
                get();
                return TOK_TIMESEQ;
            } else if (peek() == '*') {
                get();
                if (peek() == '=') {
                    get();
                    return TOK_EXPEQ;
                } else {
                    return TOK_EXP;
                }
            } else {
                return TOK_TIMES;
            }
        case '/':
            if (peek() == '=') {
                get();
                return TOK_DIVIDEEQ;
            } else {
                return TOK_DIVIDE;
            }
        case '%':
            if (peek() == '=') {
                get();
                return TOK_MODULUSEQ;
            } else {
                return TOK_MODULUS;
            }
        case '^':
            if (peek() == '=') {
                get();
                return TOK_XOREQ;
            } else {
                return TOK_BITXOR;
            }
        case '|':
            if (peek() == '=') {
                get();
                return TOK_OREQ;
            } else {
                return TOK_BITOR;
            }
        case '&':
            if (peek() == '=') {
                get();
                return TOK_ANDEQ;
            } else {
                return TOK_BITAND;
            }
        case '!':
            if (peek() != '=') {
                throw Error("Error, invalid lexeme '!'", yylineNumber);
            } else {
                get();
                return TOK_NEQ;
            }
        case '<':
            if (peek() == '=') {
                get();
                return TOK_LTE;
            } else if (peek() == '<') {
                get();
                if (peek() == '=') {
                    get();
                    return TOK_LSHIFTEQ;
                } else {
                    return TOK_LSHIFT;
                }
            } else {
                return TOK_LT;
            }
        case '>':
            if (peek() == '=') {
                get();
                return TOK_GTE;
            } else if (peek() == '>') {
                get();
                if (peek() == '=') {
                    get();
                    return TOK_RSHIFTEQ;
                } else {
                    return TOK_RSHIFT;
                }
            } else {
                return TOK_GT;
            }
        case '\\':
            if (peek() == '\n') {
                get();
                while (peek() == ' ') {
                    get();
                }

                /* recurse to skip it */
                return yylex();
            }
    }

    /* if we get down here, there must be a lexer error :( */
    char message[] = "Invalid lexeme ' '";
    message[16] = next.unicode();
    throw Error(message, yylineNumber);
    return 0;
}
