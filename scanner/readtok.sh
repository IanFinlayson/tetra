# readtok.awk converts integer output from the Tetra scanner back
# into string-token form.  You know, for testing.
# only outputs a newline on encountering a TOK_NEWLINE token.

./lexer test.tetra > test.txt
awk 'FILENAME ~ /tokens.h/ && NF == 3 {
        toks[$3] = $2
}

FILENAME ~ /test.txt/ {
        printf toks[$1] " "
}

FILENAME ~ /test.txt/ &&  /131/ {
        printf "\n"
}' tokens.h test.txt
