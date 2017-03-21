### Operator precedence levels

Below is the chart of operator precedence levels, from highest precedence to
lowest.

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

Below is a table of the types that can be used with each operator, and what type
results.

<table>
<tr><th>&nbsp;</th><th>&nbsp;</th><th>int</th><th>real</th><th>string</th><th>bool</th><th>list</th></tr>
<tr><td rowspan="5"><b>**</b></td><td><b>int</td><td>int</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>real</b></td><td>real</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>string</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>bool</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>list</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5"><b>Unary + -</b></td><td><b>int</td><td>int</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>real</b></td><td>real</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>string</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>bool</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>list</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5"><b>Unary ~</b></td><td><b>int</td><td>int</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>real</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>string</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>bool</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>list</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5"><b>%</b></td><td><b>int</td><td>int</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>real</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>string</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>bool</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>list</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5"><b>/</b></td><td><b>int</td><td>int</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>real</b></td><td>real</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>string</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>bool</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>list</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5"><b>*</b></td><td><b>int</td><td>int</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>real</b></td><td>real</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>string</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>bool</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>list</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5"><b>+</b></td><td><b>int</td><td>int</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>real</b></td><td>real</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>string</b></td><td>-</td><td>-</td><td>string</td><td>-</td><td>-</td></tr>
<tr><td><b>bool</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>list</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>list</td></tr>
<tr><td rowspan="5"><b>-</b></td><td><b>int</td><td>int</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>real</b></td><td>real</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>string</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>bool</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>list</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5"><b>&lt;&lt; &gt;&gt;</b></td><td><b>int</td><td>int</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>real</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>string</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>bool</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>list</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5"><b>&amp;</b></td><td><b>int</td><td>int</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>real</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>string</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>bool</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>list</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5"><b>^</b></td><td><b>int</td><td>int</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>real</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>string</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>bool</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>list</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5"><b>|</b></td><td><b>int</td><td>int</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>real</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>string</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>bool</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>list</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5"><b>&lt; &lt;= &gt; &gt;=</b></td><td><b>int</td><td>bool</td><td>bool</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>real</b></td><td>bool</td><td>bool</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>string</b></td><td>-</td><td>-</td><td>bool</td><td>-</td><td>-</td></tr>
<tr><td><b>bool</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>list</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5"><b>!= ==</b></td><td><b>int</td><td>bool</td><td>bool</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>real</b></td><td>bool</td><td>bool</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>string</b></td><td>-</td><td>-</td><td>bool</td><td>-</td><td>-</td></tr>
<tr><td><b>bool</b></td><td>-</td><td>-</td><td>-</td><td>bool</td><td>-</td></tr>
<tr><td><b>list</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5"><b>and</b></td><td><b>int</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>real</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>string</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>bool</b></td><td>-</td><td>-</td><td>-</td><td>bool</td><td>-</td></tr>
<tr><td><b>list</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5"><b>or</b></td><td><b>int</td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>real</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>string</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>bool</b></td><td>-</td><td>-</td><td>-</td><td>bool</td><td>-</td></tr>
<tr><td><b>list</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td rowspan="5"><b>=</b></td><td><b>int</td><td>int</td><td>int</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>real</b></td><td>real</td><td>real</td><td>-</td><td>-</td><td>-</td></tr>
<tr><td><b>string</b></td><td>-</td><td>-</td><td>string</td><td>-</td><td>-</td></tr>
<tr><td><b>bool</b></td><td>-</td><td>-</td><td>-</td><td>bool</td><td>-</td></tr>
<tr><td><b>list</b></td><td>-</td><td>-</td><td>-</td><td>-</td><td>list</td></tr>
</table>



