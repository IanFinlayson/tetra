/* syntax.js
 * apply simple syntax highlighting to Tetra code
 */

/* lists of highlighted elements */
keywords = ["class", "def", "if", "elif", "else", "while", "for", "in", "parallel", "return", "open", "import", "lambda", "background", "wait", "lock", "var", "init", "self", "constant", "none", "global"];
types = ["int", "real", "string", "bool", "task", "mutex"];
builtins = ["print", "len", "read_int", "read_string", "read_bool", "read_real", "array", "keys"];

/* apply syntax to the page */
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
            text = text.replace(re, "<span class='tetra-key'>$&</span>");
        }
        for (j = 0; j < types.length; j++) {
            var retext = "\\b" + types[j] + "\\b";
            var re = new RegExp(retext, "g");
            text = text.replace(re, "<span class='tetra-type'>$&</span>");
        }
        for (j = 0; j < builtins.length; j++) {
            var retext = "\\b" + builtins[j] + "\\b";
            var re = new RegExp(retext, "g");
            text = text.replace(re, "<span class='tetra-builtin'>$&</span>");
        }
        /* comments */
        text = text.replace(/#.*\r?\n/g, "<span class='tetra-comment'>$&</span>");
        text = text.replace(/#.*$/g, "<span class='tetra-comment'>$&</span>");

        /* strings */
        text = text.replace(/"[^"]*"/g, "<span class=\"tetra-value\">$&</span>");

        /* numbers */
        text = text.replace(/\b\d+\.\d+\b/g, "<span class=\"tetra-value\">$&</span>"); 
        text = text.replace(/\b\d+\b/g, "<span class=\"tetra-value\">$&</span>"); 
        text = text.replace(/\b(true|false)\b/g, "<span class=\"tetra-value\">$&</span>"); 

        /* write it back */
        snippets[i].innerHTML = text;
    }
}

