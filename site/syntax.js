/* syntax.js
 * apply simple syntax highlighting to Tetra code
 */

keywords = ["def", "if", "elif", "else", "while", "for", "in", "parallel"];

function applySyntax() {
    /* find all the <pre><code> blocks */
    var snippets = document.querySelectorAll('pre code');

    /* highlight each one */
    for (i = 0; i < snippets.length; i++) {
        /* get existing text */
        var text = snippets[i].textContent;
        
        /* apply the replacements */
        for (j = 0; j < keywords.length; j++) {
            var retext = "\\b" + keywords[j] + "\\b";
            var re = new RegExp(retext, "g");
            text = text.replace(re, "<span class=\"tetra-key\">$&</span>");
        }

        /* write it back */
        snippets[i].textContent = text;
    }
}

