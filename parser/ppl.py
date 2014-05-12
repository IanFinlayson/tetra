#!/usr/bin/env python3

import fileinput

# this file is just used for testing the lexer...

text = [
    "IF",
    "ELIF",
    "ELSE",
    "FOR",
    "IN",
    "FORALL",
    "PARALLEL",
    "WHILE",
    "CONTINUE",
    "BREAK",
    "DEF",
    "GLOBAL",
    "OR",
    "AND",
    "NOT",
    "PASS",
    "RETURN",
    "INT",
    "REAL",
    "BOOL",
    "STRING",
    "ASSIGN",
    "PLUS",
    "MINUS",
    "TIMES",
    "DIVIDE",
    "MODULUS",
    "BITXOR",
    "BITAND",
    "BITOR",
    "BITNOT",
    "LEFTPARENS",
    "RIGHTPARENS",
    "COMMA",
    "DOLLAR",
    "SEMICOLON",
    "COLON",
    "LSHIFT",
    "RSHIFT",
    "EXP",
    "LTE",
    "GTE",
    "EQ",
    "NEQ",
    "LT",
    "GT",
    "PLUSEQ",
    "MINUSEQ",
    "TIMESEQ",
    "DIVIDEEQ",
    "MODULUSEQ",
    "EXPEQ",
    "RSHIFTEQ",
    "LSHIFTEQ",
    "ANDEQ",
    "XOREQ",
    "OREQ",
    "INTVAL",
    "REALVAL",
    "BOOLVAL",
    "STRINGVAL",
    "IDENTIFIER",
    "INDENT",
    "DEDENT",
    "NEWLINE"]

for line in fileinput.input( ):
    num = int(line) - 258
    if num == -258:
        print("EOF")
        break
    print(text[num], end=" ")
    if text[num] == "NEWLINE":
        print("")


