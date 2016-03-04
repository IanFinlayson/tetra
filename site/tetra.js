/* tetra.js
 * automatically highlight all Tetra code on the site
 * started from this example on SO:
 * http://stackoverflow.com/questions/2155588/
 */

function tetra_highlight() { 
   var codes= document.getElementsByTagName('code')
      ,str= ''
      ,len= codes.length
      ,c= 0
      ;
   for(c; c < len; c++){ 
      str = codes[c].textContent;   
      str=str.replace(/(\bnull\b|\b[0-9]+\b|\btrue\b|\bfalse\b|"(?:[^"\\]|\\.)*"|'(?:[^'\\]|\\.)*')/g,"<span class=\"tetra_val\">$1</span>");
      str=str.replace(/(\(|\)|\[|\]|\{|\})/g,"<span class='tetra_bracket'>$&</span>");
      str=str.replace(/\$[a-z]\w*/gi,"<span class='tetra_var'>$&</span>");
      str=str.replace(/( \! | \!\= | \!== | = | == | === | > | >= | < | <= | and | or )/g,"<span class='tetra_op'>$&</span>");
      str=str.replace(/\b(for|function|return|unset|count|global|if|else|else if|intval|int|array)\b/g,"<span class='tetra_key'>$1</span>");
      codes[c].innerHTML=str;
   }
}

