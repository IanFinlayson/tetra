/* syntax.js
 * apply simple syntax highlighting to Tetra code
 */

keywords = ["def", "if", "elif", "else", "while", "for", "in", "parallel", "return", "open", "import", "lambda", "background", "wait", "lock", "class", "construct"];
types = ["int", "real", "string", "bool", "task", "mutex"];
builtins = ["print", "len", "read_int"];

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
        for (j = 0; j < types.length; j++) {
            var retext = "\\b" + types[j] + "\\b";
            var re = new RegExp(retext, "g");
            text = text.replace(re, "<span class=\"tetra-type\">$&</span>");
        }
        for (j = 0; j < builtins.length; j++) {
            var retext = "\\b" + builtins[j] + "\\b";
            var re = new RegExp(retext, "g");
            text = text.replace(re, "<span class=\"tetra-builtin\">$&</span>");
        }


        /* write it back */
        snippets[i].innerHTML = text;
    }
}

