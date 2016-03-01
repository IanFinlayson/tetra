<?php

/*
site.php

This file drives the whole web site.  It includes the various structural
elements:

 - preamble.html which contains the <head> tag with links to fonts etc.
 - nav.html which contains the logo at the top and menu bar
 - sidebar.html which contains the list of links etc. on the right
 - postamble.html which contains the footer and closing body tag

This file takes the name of the page to display as a GET parameter called
"page".  It takes the value of this and looks for a html file in the content
directory with the same name.  If found, it dumps it in the middle of the page.
*/

include 'structure/preamble.html';
include 'structure/nav.html';

echo '<main>
<div class="main">';

$msg404 = '<h1>Page not found!</h1>
<p>There does not seem to be anything here.  If you got here
from a broken link, or if something is missing, please <a href="contact">
let us know.</p>';

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

echo '</main></div>';

include 'structure/sidebar.html';
include 'structure/postamble.html';

?>

