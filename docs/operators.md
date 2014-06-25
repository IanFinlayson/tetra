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
<tr><td rowspan="5">**</b></td><td><b>int</td><td>int</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>real</b></td><td>real</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>string</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>bool</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>vector</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5">Unary + -</b></td><td><b>int</td><td>int</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>real</b></td><td>real</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>string</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>bool</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>vector</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5">Unary ~</b></td><td><b>int</td><td>int</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>real</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>string</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>bool</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>vector</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5">%</b></td><td><b>int</td><td>int</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>real</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>string</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>bool</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>vector</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5">/</b></td><td><b>int</td><td>int</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>real</b></td><td>real</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>string</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>bool</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>vector</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5">*</b></td><td><b>int</td><td>int</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>real</b></td><td>real</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>string</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>bool</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>vector</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5">+</b></td><td><b>int</td><td>int</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>real</b></td><td>real</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>string</b></td><td>-</td><td>-</td><td>string</td><td>-</td><td>-</td></tr>
<tr><td><b>bool</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>vector</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>vector</td></tr>
<tr><td rowspan="5">-</b></td><td><b>int</td><td>int</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>real</b></td><td>real</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>string</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>bool</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>vector</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5">&lt;&lt; &gt;&gt;</b></td><td><b>int</td><td>int</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>real</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>string</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>bool</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>vector</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5">&amp;</b></td><td><b>int</td><td>int</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>real</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>string</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>bool</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>vector</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5">^</b></td><td><b>int</td><td>int</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>real</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>string</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>bool</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>vector</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5">|</b></td><td><b>int</td><td>int</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>real</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>string</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>bool</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>vector</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5">&lt; &lt;= &gt; &gt;=</b></td><td><b>int</td><td>bool</td><td>bool</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>real</b></td><td>bool</td><td>bool</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>string</b></td><td>-</td><td>-</td><td>bool</td><td>-</td><td>-</td></tr>
<tr><td><b>bool</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>vector</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5">!= ==</b></td><td><b>int</td><td>bool</td><td>bool</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>real</b></td><td>bool</td><td>bool</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>string</b></td><td>-</td><td>-</td><td>bool</td><td>-</td><td>-</td></tr>
<tr><td><b>bool</b></td><td>-</td><td>-</td><td>-</td><td>bool</td><td>-</td></tr>
<tr><td><b>vector</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5">and</b></td><td><b>int</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>real</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>string</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>bool</b></td><td>-</td><td>-</td><td>-</td><td>bool</td><td>-</td></tr>
<tr><td><b>vector</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5">or</b></td><td><b>int</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>real</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>string</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>bool</b></td><td>-</td><td>-</td><td>-</td><td>bool</td><td>-</td></tr>
<tr><td><b>vector</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5">=</b></td><td><b>int</td><td>int</td><td>int</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>real</b></td><td>real</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>string</b></td><td>-</td><td>-</td><td>string</td><td>-</td><td>-</td></tr>
<tr><td><b>bool</b></td><td>-</td><td>-</td><td>-</td><td>bool</td><td>-</td></tr>
<tr><td><b>vector</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>vector</td></tr>
</table>



