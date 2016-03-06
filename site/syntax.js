/* syntax.js
 * apply simple syntax highlighting to Tetra code
 */

keywords = ["def", "if", "elif", "else", "while", "for", "in", "parallel"];

function applySyntax() {
    /* find all the <pre><code> blocks */
    var snippets = document.querySelectorAll('pre code');

    snippets[0].style.backgroundColor = "red"; 

    /* highlight each one */
    for (i = 0; i < snippets.length; i++) {
        var text = snippets[i].textContent;
        snippets[i].textContent = "Heyyo";
        alert(text);
        //highlighted = text.replace(/def/g, "heyoo");
        //snippet.innerHTML = highlighted;
        //alert(highlighted);
    }
}

