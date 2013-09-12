# takes a list of tokens for the Tetra scanner and generates a definition 
# file for them (printed to stdout)

BEGIN {
    FS = "\N"
    RS = ""
    tvalue = 100

    print "/* A list of tokens for the Tetra scanner */"
    print "#ifndef H_TETRA_TOKENS"
    print "#define H_TETRA_TOKENS\n"
}

NF > 0 {
    print "#define TOK_" toupper($1) " " tvalue++
}

END {
    print "\n#endif"
}
