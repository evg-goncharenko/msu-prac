## Functional programming

*Read this in other languages: [English](README.md), [Русский](README.ru.md).*

### Practicum
- Solving a system of linear equations in the Russian version ([slae_ru](./slae_ru.ref));
- Solving a system of linear equations in the English version ([slae](./slae.ref)).

### Homework assignment
- Exclude from the text all words that contain at least one digit ([delete_words](./delete_words.ref)).

---

## Solving a system of linear equations

Building a Refal2:
```bash
# Creating a directory `build`:
mkdir build; cd build
# For Windows with `Microsoft Visual Studio 2010`: cmake -G "Visual Studio 10 2010" ../
# For MacOS with `Xcode IDE`: cmake -G "Xcode" ../
# To see the available options: cmake -G
cmake -G "Xcode" ../
# Build:
cmake --build .
```

To run the program, you need to put, for example, "slae.ref" at `refal/build/Debug`, where is "./refal2":
```bash
./refal2 slae.ref
```

### Problem statement:

A system of `n` linear equations with `m` unknowns is given, `n` and `m` ≥ 1, for example (m = 2, n = 2):
```none
3 * x + 2 * x - 16 / (1 + 7) * y = 11
x + 2 * y = 7
```

Coefficients for equation variables can be both numbers and arithmetic expressions made up of them (using multiplication, addition, subtraction, division and parentheses).<br>
It is necessary to create a refal program that solves this system by eliminating variables.<br>

In general, the solution of the system includes the following steps:
1) All equations of the system are transformed to the canonical form:<br>`a * x + b * y + c * z + ... + d = 0`,
where `a`, `b`, `c`, `d` – numbers.
2) In one of the equations, some variable is expressed through other variables:<br>
`x = - (b * y + c * z + ... + d)/a = - b/a * y - c/a * z - ... - d/a`.
3) The expression obtained for this variable is substituted into the rest of the equations of the system instead of all occurrences
of the specified variable.<br>

As a result of these three steps, a formula is obtained for calculating the value of one of the system variables from the values of other
variables, as well as a new equivalent system of equations containing one equation and one variable less than the previous one. Next,<br>
steps 1-3 are repeated for this new system of equations, and this continues until one equation remains or a contradiction is discovered.<br>
A contradiction (incorrect equality) occurs when the system has no solution (for example, during the transformations, the equation<br>
`6 - 4 = 0` is obtained).<br>
In this case, the refal program should give a message as a response: `The system of equations has no solution`.
If during the transformations one of the equations turned into an identity (0=0), then the original system of equations was redundant,
and this equation can be excluded from the system being solved.<br>
If one equation with one variable is obtained during the transformations of the system, then the value of this variable is calculated from
this equation. The values of the remaining variables are calculated using the formulas obtained during the transformations. In this case,
the refal program should output the solution of the system of equations in the form of strings of the form `variable name = value`,
for example, for the system of equations given above as an example, the answer will be:
```none
Result:
x = 3
y = 2
```
If, as a result of transformations of the system of equations, one equation remains, but it contains more than one variable<br>
(for example `x + y + 2 = 0`), then this means that the original system has an infinite set of solutions. In this case, the refal program
should print the names of free variables (which can take any values) and formulas for calculating the remaining variables of
the system based on the values of these free variables. For example, for a given system of equations:
```none
x + 2 * y - z = 2
2 * x - y + 3 * z = -6
```
the refal program gives the result:
```none
Result:
x = -2-z
y = 2+z
z - free variable
```
