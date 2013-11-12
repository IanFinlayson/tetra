BEGIN {
    while ((getline < "parser.y") > 0) {
        if ($1 ~ /token/ && $2 ~ /<[[:alpha:]]+>/) {
            gsub("TOK_", "N_")
            gsub(/[<>]/, "", $2)
            if (NF > 2) {
                for (i = 3; i <= NF; i++) {
                    type[$i] = $2
                }
            }
        }
    }
    for (i in type)
        print i ": " type[i]
}

/Node_Types/ { p = 1 }
/^}/ { p = 0 }

p && /[[:upper:]_]+/ && !/\/\*/ && !/enum/ {
    gsub(/,/, "")
    if (type[$1] ~ /i/) {
        format = "%d"
        value = ", N_INT(node)"
    } else if (type[$1] ~ /f/) {
        format = "%f"
        value = ", N_FLOAT(node)"
    } else if (type[$1] ~ /text/) {
        format = "%s"
        value = ", N_STR(node)"
    } else {
        format = ""
        value = ""
    }
    printf "case " $1
    sub(/N_/, "")
    printf ": printf(\"%s: %s\"%s); break;\n", $1, format, value
}
