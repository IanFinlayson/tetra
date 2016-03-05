/* syntax.js
 * apply simple syntax highlighting to Tetra code
 */

keywords = ["def", "if", "elif", "else", "while", "for", "in", "parallel"];

function applySyntax() {
    /* find all the <pre><code> blocks */
    var snippets = document.querySelectorAll('pre code');

    /* highlight each one */
    for (snippet in snippets) {
        var text = node.textContent;
        highlighted = text.replace(/def/g, "heyoo");
        snippet.innerHTML = highlighted;
    }
}

