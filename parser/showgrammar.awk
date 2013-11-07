# This script extracts a grammar from a bison file for easier reading
BEGIN {
    RS = ""
    FS = "|"
    show = 0
}

/%%/ { 
    show = 1 - show
    next
}

{
    if (!show || in_comment)
        next
    gsub("{[^}]*}", "")
    gsub("[\t ]+", " ")
    gsub("[\n]", "")
    print $1
    if (NR > 1) {
        for (i = 2; i <= NF; i++) {
            print "\t|", $i
        }
    }
    print ""
}

END {

}
