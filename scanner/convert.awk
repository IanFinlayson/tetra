# Takes a list of tokens for the Tetra scanner and generates a definition 
# File for them (printed to stdout)
# To test, invoke with 'awk -f mktok.awk ttoks.txt'

BEGIN {
    FS = "\n"
    RS = ""
    print "# A list of tokens for the Tetra scanner"
}

/^#/ {
    next
}

NF > 0 {
    print toupper($1)
}

END {
}
