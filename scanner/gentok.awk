# Takes a list of tokens for the Tetra scanner and generates a definition 
# File for them (printed to stdout)
# To test, invoke with 'awk -f mktok.awk toks.txt'

BEGIN {
    tvalue = 100

    print "/* A list of tokens for the Tetra scanner */"
    print "#ifndef H_TETRA_TOKENS"
    print "#define H_TETRA_TOKENS\n"
    print "#define TOK_NULL 1"
} 
/^#/ {
    next
}

{
    print "#define TOK_" toupper($1) " " tvalue++
}

END {
    print "\n#endif"
}
