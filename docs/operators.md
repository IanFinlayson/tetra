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
<tr><th>&nbsp;</th><th>&nbsp;</th><th>int</th><th>real</th><th>string</th><th>bool</th><th>vector</th></tr>
<tr><th rowspan="5">**</th><td>int</td><td>int</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>real</th><td>real</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>string</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>bool</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>vector</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th rowspan="5">Unary + -</th><td>int</td><td>int</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>real</th><td>real</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>string</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>bool</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>vector</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th rowspan="5">Unary ~</th><td>int</td><td>int</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>real</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>string</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>bool</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>vector</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th rowspan="5">%</th><td>int</td><td>int</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>real</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>string</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>bool</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>vector</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th rowspan="5">/</th><td>int</td><td>int</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>real</th><td>real</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>string</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>bool</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>vector</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th rowspan="5">*</th><td>int</td><td>int</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>real</th><td>real</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>string</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>bool</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>vector</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th rowspan="5">+</th><td>int</td><td>int</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>real</th><td>real</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>string</th><td>-</td><td>-</td><td>string</td><td>-</td><td>-</td></tr>
<tr><th>bool</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>vector</th><td>-</td><td>-</td><td>-</td><td>-</td><td>vector</td></tr>
<tr><th rowspan="5">-</th><td>int</td><td>int</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>real</th><td>real</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>string</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>bool</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>vector</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th rowspan="5">&lt;&lt; &gt;&gt;</th><td>int</td><td>int</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>real</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>string</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>bool</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>vector</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th rowspan="5">&amp;</th><td>int</td><td>int</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>real</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>string</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>bool</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>vector</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th rowspan="5">^</th><td>int</td><td>int</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>real</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>string</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>bool</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>vector</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th rowspan="5">|</th><td>int</td><td>int</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>real</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>string</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>bool</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>vector</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th rowspan="5">&lt; &lt;= &gt; &gt;=</th><td>int</td><td>bool</td><td>bool</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>real</th><td>bool</td><td>bool</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>string</th><td>-</td><td>-</td><td>bool</td><td>-</td><td>-</td></tr>
<tr><th>bool</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>vector</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th rowspan="5">!= ==</th><td>int</td><td>bool</td><td>bool</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>real</th><td>bool</td><td>bool</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>string</th><td>-</td><td>-</td><td>bool</td><td>-</td><td>-</td></tr>
<tr><th>bool</th><td>-</td><td>-</td><td>-</td><td>bool</td><td>-</td></tr>
<tr><th>vector</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th rowspan="5">and</th><td>int</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>real</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>string</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>bool</th><td>-</td><td>-</td><td>-</td><td>bool</td><td>-</td></tr>
<tr><th>vector</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th rowspan="5">or</th><td>int</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>real</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>string</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>bool</th><td>-</td><td>-</td><td>-</td><td>bool</td><td>-</td></tr>
<tr><th>vector</th><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th rowspan="5">=</th><td>int</td><td>int</td><td>int</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>real</th><td>real</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><th>string</th><td>-</td><td>-</td><td>string</td><td>-</td><td>-</td></tr>
<tr><th>bool</th><td>-</td><td>-</td><td>-</td><td>bool</td><td>-</td></tr>
<tr><th>vector</th><td>-</td><td>-</td><td>-</td><td>-</td><td>vector</td></tr>
</table>



