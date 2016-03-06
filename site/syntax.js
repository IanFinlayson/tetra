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
        for (k in keywords) {
            var re = new RegExp(k, "g");
            text = text.replace(re, "<bold>\1</bold>");
        }

        /* write it back */
        snippets[i].textContent = text;
    }
}

