### Operator precedence levels

Operator           | Meaning
-------------------|---------
**                 | Exponentiation
+ - ~              | Unary plus, unary minus, and unary bitwise not
% / *              | Modulus, division, multiplication
+ -                | Addition and subtraction
<< >>              | Bitwise left shift and right shift
&                  | Bitwise and
^                  | Bitwise xor
&#124;             | Bitwise or
< <= > >= == !=    | Comparison operators
and                | Logical and
or                 | Logical or
=                  | Assignment (including +=, -= etc)

### Operator/Type Chart



<table>
<tr><th></th><th>&nbsp;</th><th>int</th><th>real</th><th>string</th><th>bool</th><th>vector</th></tr>
<tr><td>**</td><td>int</td><td>int</td><td>real</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>real</td><td>real</td><td>real</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>string</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>bool</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>vector</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>Unary + -</td><td>int</td><td>int</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>
<tr><td>&nbsp;</td><td>real</td><td>real</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>
<tr><td>&nbsp;</td><td>string</td><td>NA</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>
<tr><td>&nbsp;</td><td>bool</td><td>NA</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>
<tr><td>&nbsp;</td><td>vector</td><td>NA</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>
<tr><td>Unary ~</td><td>int</td><td>int</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>
<tr><td>&nbsp;</td><td>real</td><td>NA</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>
<tr><td>&nbsp;</td><td>string</td><td>NA</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>
<tr><td>&nbsp;</td><td>bool</td><td>NA</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>
<tr><td>&nbsp;</td><td>vector</td><td>NA</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr>
<tr><td>%</td><td>int</td><td>int</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>real</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>string</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>bool</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>vector</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>/</td><td>int</td><td>int</td><td>real</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>real</td><td>real</td><td>real</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>string</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>bool</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>vector</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>*</td><td>int</td><td>int</td><td>real</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>real</td><td>real</td><td>real</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>string</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>bool</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>vector</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>+</td><td>int</td><td>int</td><td>real</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>real</td><td>real</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>string</td><td>NA</td><td>NA</td><td>string</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>bool</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>vector</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>vector</td></tr>
<tr><td>-</td><td>int</td><td>int</td><td>real</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>real</td><td>real</td><td>real</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>string</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>bool</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>vector</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td><< >></td><td>int</td><td>int</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>real</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>string</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>bool</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>vector</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&</td><td>int</td><td>int</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>real</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>string</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>bool</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>vector</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>^</td><td>int</td><td>int</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>real</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>string</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>bool</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>vector</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>|</td><td>int</td><td>int</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>real</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>string</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>bool</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>vector</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>< <= > >=</td><td>int</td><td>bool</td><td>bool</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>real</td><td>bool</td><td>bool</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>string</td><td>NA</td><td>NA</td><td>bool</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>bool</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>vector</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>!= ==</td><td>int</td><td>bool</td><td>bool</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>real</td><td>bool</td><td>bool</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>string</td><td>NA</td><td>NA</td><td>bool</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>bool</td><td>NA</td><td>NA</td><td>NA</td><td>bool</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>vector</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>and</td><td>int</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>real</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>string</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>bool</td><td>NA</td><td>NA</td><td>NA</td><td>bool</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>vector</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>or</td><td>int</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>real</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>string</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>bool</td><td>NA</td><td>NA</td><td>NA</td><td>bool</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>vector</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>=</td><td>int</td><td>int</td><td>int</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>real</td><td>real</td><td>real</td><td>NA</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>string</td><td>NA</td><td>NA</td><td>string</td><td>NA</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>bool</td><td>NA</td><td>NA</td><td>NA</td><td>bool</td><td>NA</td></tr>
<tr><td>&nbsp;</td><td>vector</td><td>NA</td><td>NA</td><td>NA</td><td>NA</td><td>vector</td></tr>
<tr><td>&nbsp;</td><td>&nbsp;</td><td>int</td><td>real</td><td>string</td><td>bool</td><td>vector</td></tr>
</table>



