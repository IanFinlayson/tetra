<?php

/* include the preamble (head and imports etc.)
 * and the nav (the top bar on all pages) */
include 'structure/preamble.html';
include 'structure/nav.html';


$msg404 = '<div class="container"><h1>Page not found!</h1>
<p>There does not seem to be anything here.
You can try to go back to <a href="/">the home page</a>.
</p></div>';

if ($_GET["page"]) {
    /* trim off the / if present! */
    $pagename = rtrim($_GET["page"], '/');

    $fname = 'content/' . $pagename . '.html';

    if (file_exists($fname)) {
        include $fname;
    } else {
        echo $msg404;
    }
} else {
    echo $msg404;
}

include 'structure/postamble.html';

?>

