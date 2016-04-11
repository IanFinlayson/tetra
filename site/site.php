<!DOCTYPE html>
<html lang="en">
<head>
<title>Tetra Programming Language</title>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<meta name="description" content="Tetra Programming Language Site">
<meta name="keywords" content="Tetra, Parallel Programming">
<meta name="author" content="Ian Finlayson">

<!-- load fonts -->
<link href='https://fonts.googleapis.com/css?family=Fira+Sans:400,700,500,400italic,500italic,700italic,300italic,300|Fira+Mono:400,700' rel='stylesheet' type='text/css'>

<!-- load styles sheets -->
<link rel="stylesheet" href="style/theme.min.css">
<link rel="stylesheet" href="style/code.css">

<!-- load scripts -->
<script src="scripts/jquery.min.js"></script>
<script src="scripts/bootstrap.min.js"></script>
<script src="scripts/syntax.js"></script>
<script src="scripts/preCode.js"></script>
<script src="scripts/smartquotes.min.js"></script>

<!-- favicons -->
<link rel="apple-touch-icon-precomposed" sizes="57x57" href="/images/apple-touch-icon-57x57.png" />
<link rel="apple-touch-icon-precomposed" sizes="114x114" href="/images/apple-touch-icon-114x114.png" />
<link rel="apple-touch-icon-precomposed" sizes="72x72" href="/images/apple-touch-icon-72x72.png" />
<link rel="apple-touch-icon-precomposed" sizes="144x144" href="/images/apple-touch-icon-144x144.png" />
<link rel="apple-touch-icon-precomposed" sizes="60x60" href="/images/apple-touch-icon-60x60.png" />
<link rel="apple-touch-icon-precomposed" sizes="120x120" href="/images/apple-touch-icon-120x120.png" />
<link rel="apple-touch-icon-precomposed" sizes="76x76" href="/images/apple-touch-icon-76x76.png" />
<link rel="apple-touch-icon-precomposed" sizes="152x152" href="/images/apple-touch-icon-152x152.png" />
<link rel="icon" type="image/png" href="/images/favicon-196x196.png" sizes="196x196" />
<link rel="icon" type="image/png" href="/images/favicon-96x96.png" sizes="96x96" />
<link rel="icon" type="image/png" href="/images/favicon-32x32.png" sizes="32x32" />
<link rel="icon" type="image/png" href="/images/favicon-16x16.png" sizes="16x16" />
<link rel="icon" type="image/png" href="/images/favicon-128.png" sizes="128x128" />
<meta name="application-name" content="Tetra Site"/>
<meta name="msapplication-TileColor" content="#FFFFFF" />
<meta name="msapplication-TileImage" content="/images/mstile-144x144.png" />
<meta name="msapplication-square70x70logo" content="/images/mstile-70x70.png" />
<meta name="msapplication-square150x150logo" content="/images/mstile-150x150.png" />
<meta name="msapplication-square310x310logo" content="/images/mstile-310x310.png" />

<script>
/* on page load, we apply smart quotes, then do syntax highlighting */
function initPage() {
    smartquotes();
    applySyntax();
}

/* make the search box work */
function doSearch() {
    url = "https://www.google.com/search?q=" + document.getElementById("searchbox").value + "%20site%3Atetra-lang.org";
    window.location = url;
}
</script>
</head>

<body onload="initPage()">


<!-- the navigation bar on top of the page -->
<nav role="navigation" class="navbar navbar-default">

    <div class="navbar-header">
        <!-- the collapsed menu thing -->
        <button type="button" data-target="#navbarCollapse" data-toggle="collapse" class="navbar-toggle">
            <span class="sr-only">Toggle navigation</span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
        </button>

        <!-- the logo in the upper left -->
        <a href="/" class="pull-left"><img src="images/header.svg" width="115" class="img-responsive" /></a>
    </div>

    <!-- the full menu content -->
    <div id="navbarCollapse" class="collapse navbar-collapse">
       <ul class="nav navbar-nav">
            <!-- single items -->
            <li><a href="news">News</a></li>
            <li><a href="download">Downloads</a></li>

            <!-- learn menu -->
            <li class="dropdown">
                <a data-toggle="dropdown" class="dropdown-toggle" href="#">Learn<b class="caret"></b></a>
                <ul role="menu" class="dropdown-menu">
                    <li><a href="overview">Overview</a></li>
                    <li><a href="tutorial">Tutorial</a></li>
                    <li><a href="book">Book</a></li>
                </ul>
            </li>

            <!-- docs menu -->
            <li class="dropdown">
                <a data-toggle="dropdown" class="dropdown-toggle" href="#">Documentation<b class="caret"></b></a>
                <ul role="menu" class="dropdown-menu">
                    <li><a href="lang-reference">Language Reference</a></li>
                    <li><a href="lib-reference">Library Reference</a></li>
                    <li><a href="papers">Papers</a></li>
                </ul>
            </li>

            <!-- contribute menu -->
            <li class="dropdown">
                <a data-toggle="dropdown" class="dropdown-toggle" href="#">Contribute<b class="caret"></b></a>
                <ul role="menu" class="dropdown-menu">
                    <li><a href="https://github.com/IanFinlayson/tetra">Source Code</a></li>
                    <li><a href="https://github.com/IanFinlayson/tetra/issues">Issue Tracker</a></li>
                    <li><a href="qa">Q &amp; A Site</a></li>
                </ul>
            </li>
            </ul>

            <!-- search box -->
            <ul class="nav navbar-nav navbar-right">
            <li class="dropdown">
                <a data-toggle="dropdown" class="dropdown-toggle" href="#"><span class="glyphicon glyphicon-search"></span><b class="caret"></b></a>
              <ul role="menu" class="dropdown-menu">
                <li>
                  <form class="navbar-form  " role="search" onSubmit="javascript:doSearch()" action="javascript:void(0)">
                    <div class="form-group search-phone2">
                      <input type="text" class="form-control" placeholder="Search" name="search" id="searchbox">
                    </div>      
                  </form>
                </li>
              </ul>
            </li>

        <!-- end of menu content -->
        </ul>
    </div>

<!-- end of the top bar -->
</nav>



<?php

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

?>


<!-- footer -->
<p class="text-center"><small>Copyright &copy; 2016 Tetra | Licensed under the MIT License</small></p>


</body>
</html>

