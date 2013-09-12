# Generates skeleton for the PATTERN { ACTION } section of a lex file.
# Takes the same token file as is passed to mktok.awk

BEGIN {
    FS = "\n"
    RS = ""
    print "#.*\t/* comments */"
}

NF == 1 {
    print "PATTERNHERE\t{ return TOK_" toupper($1) "; }"
}

NF == 2 {
    print $2 "\t{ return TOK_" toupper($1) "; }"
}

NF > 2 {
    print "(?x: " $2
    for (i = 3; i < NF; i++) {
        print "   | " $i
    }
    print "   | " $NF ")\t{ return TOK_" toupper($1) "; }"
}

#NF > 2 {
#    print "Malformed token file" | "cat >&2" 
#    exit(1)
#}
