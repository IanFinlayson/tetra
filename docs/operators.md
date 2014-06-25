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
<tr><td rowspan="5">**</td><td>int</td><td>int</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>real</td><td>real</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>string</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>bool</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>vector</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5">Unary + -</td><td>int</td><td>int</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>real</td><td>real</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>string</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>bool</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>vector</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5">Unary ~</td><td>int</td><td>int</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>real</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>string</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>bool</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>vector</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5">%</td><td>int</td><td>int</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>real</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>string</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>bool</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>vector</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5">/</td><td>int</td><td>int</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>real</td><td>real</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>string</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>bool</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>vector</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5">*</td><td>int</td><td>int</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>real</td><td>real</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>string</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>bool</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>vector</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5">+</td><td>int</td><td>int</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>real</td><td>real</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>string</td><td>-</td><td>-</td><td>string</td><td>-</td><td>-</td></tr>
<tr><td>bool</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>vector</td><td>-</td><td>-</td><td>-</td><td>-</td><td>vector</td></tr>
<tr><td rowspan="5">-</td><td>int</td><td>int</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>real</td><td>real</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>string</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>bool</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>vector</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5">&lt;&lt; &gt;&gt;</td><td>int</td><td>int</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>real</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>string</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>bool</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>vector</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5">&amp;</td><td>int</td><td>int</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>real</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>string</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>bool</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>vector</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5">^</td><td>int</td><td>int</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>real</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>string</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>bool</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>vector</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5">|</td><td>int</td><td>int</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>real</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>string</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>bool</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>vector</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5">&lt; &lt;= &gt; &gt;=</td><td>int</td><td>bool</td><td>bool</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>real</td><td>bool</td><td>bool</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>string</td><td>-</td><td>-</td><td>bool</td><td>-</td><td>-</td></tr>
<tr><td>bool</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>vector</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5">!= ==</td><td>int</td><td>bool</td><td>bool</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>real</td><td>bool</td><td>bool</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>string</td><td>-</td><td>-</td><td>bool</td><td>-</td><td>-</td></tr>
<tr><td>bool</td><td>-</td><td>-</td><td>-</td><td>bool</td><td>-</td></tr>
<tr><td>vector</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5">and</td><td>int</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>real</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>string</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>bool</td><td>-</td><td>-</td><td>-</td><td>bool</td><td>-</td></tr>
<tr><td>vector</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5">or</td><td>int</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>real</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>string</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>bool</td><td>-</td><td>-</td><td>-</td><td>bool</td><td>-</td></tr>
<tr><td>vector</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5">=</td><td>int</td><td>int</td><td>int</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>real</td><td>real</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td>string</td><td>-</td><td>-</td><td>string</td><td>-</td><td>-</td></tr>
<tr><td>bool</td><td>-</td><td>-</td><td>-</td><td>bool</td><td>-</td></tr>
<tr><td>vector</td><td>-</td><td>-</td><td>-</td><td>-</td><td>vector</td></tr>
</table>


