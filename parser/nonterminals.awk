BEGIN {
    opener = "%%type <TTR_Node> "
    opener_len = length(opener)
    max_len = 75 # I like to leave room for line numbers
    len = opener_len

    in_comment = 0

    printf opener
}

/\/\*[^*]*$/ { in_comment = 1 }
/^[^\/]*\*\/[ \t]*$/ { in_comment = 0 }

$1 ~ /[[:alpha:]-]+:/ {
    if (in_comment) next
    sub(":", " ", $1)
    if (len + length($1) > max_len) {
        print ""
        printf opener
        len = opener_len
    }
    printf $1
    len += length($1)
}

END {
    print ""
}
