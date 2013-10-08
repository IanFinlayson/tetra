BEGIN {
    opener = "%%type <node> "
    opener_len = length(opener)
    max_len = 75 # I like to leave room for line numbers
    len = opener_len

    printf opener
}

$1 ~ /[[:alpha:]-]:/ {
    sub(":", " ", $1)
    if (len + length($1) > max_len) {
        print ""
        printf opener
        len = opener_len
    }
    printf $1
    len += length($1)
}
