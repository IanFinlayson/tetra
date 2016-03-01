
<?php

include 'structure/preamble.html';
include 'structure/nav.html';


echo '<div class="main">';

if ($_GET["page"]) {
    $fname = 'content/' . $_GET["page"] . '.html';

    if (file_exists($fname)) {
        include $fname;
    } else {
        echo '<h1>Whoops, there doesn\'t seem to be anything here :(</h1>';
    }
} else {
    echo '<h1>Whoops, there doesn\'t seem to be anything here :(</h1>';
}

echo '</div>';

include 'structure/sidebar.html';
include 'structure/postamble.html';

?>

