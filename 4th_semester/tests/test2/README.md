## Test No. 2

*Read this in other languages: [English](README.md), [Русский](README.ru.md).*

<b> [v1_1](./v1_1.cpp): </b><br>

A grammar is given that generates strings, brackets, integers and two operation signs - `*` и `+`:

```none
S -> "L | dD | + | * | ( | )
D -> dD | eps
L -> cL | "
```

Nonterminals - `S`, `L`, `D`. Terminals - `+`, `*`, `"`, `(`, `)`, `с` (`c` is any character - not quotes), `d` (any number from 0 to 9). Implement a lexical analyzer for this grammar in the form of a `Scanner` class, which should contain the closed method `gc()` , reading a character from the input chain, and closed method `step()`, executing one step of the state machine operation, as well as one open method - `getToken()`, returning the value of a `Token` class. Develop the structure of the `Token` class yourself. The input chain is taken either from the argument of the class constructor, or from the input stream (then the default constructor is used). <br>

<br> <b> [v1_2](./v1_2.cpp): </b><br>

Using the `Scanner` class from task 1, write a parser using the recursive descent method for a grammar describing string expressions:

```none
S -> T { + T }
T -> F { * d }
F -> s | ( S )
```

Here `d` and `s` are tokens (`Token`) - numbers and strings from task 1, respectively. The program should print either `success` on the screen if it matches the grammar, or an error message. <br>

<br> <b> [v1_3](./v1_3.cpp): </b><br>

In analyzer program from task 2, insert actions to calculate the value of expression described by the grammar from task 2. The value of the expression is a string, `+` is considered as a string concatenation operation, and `s * n` is considered as a string `s` repeated `n` times. The resulting program should output either an error message in the original expression, or a calculated value. <br><br>

<b>Examples:</b>

```none
"abc" + "cde" -> abccde
"abc" * 3 * 2 -> abcabcabcabcabcabc
```
