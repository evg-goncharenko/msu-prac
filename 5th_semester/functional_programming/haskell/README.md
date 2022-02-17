## Haskell Test

*Read this in other languages: [English](README.md), [Русский](README.ru.md).*

<b>[task1](./task1.hs):</b><br>

Declare a synonym of the type to represent a point in three-dimensional space and implement a function that
calculates the volume of the cone `πhR²/3` by a given center of the base, vertex and radius of the base of the cone.
Determine the necessary auxiliary functions if their use improves the visibility of the solution.

<br><b>[task2](./task2.hs):</b><br>

For the type `data Tree = Empty | Node Int Tree Tree`, define the function `mulTrees`, which takes two trees as
arguments and returns a tree containing in each node the product of the values in the corresponding nodes of
the source trees. A node in resulting tree must be present if the corresponding node was present in at least one
of the source trees (for missing nodes, their value can be considered equal to 1).

<br><b>[task3](./task3.hs):</b><br>

Using the functionals, describe the function `findNames`, which in a given list of tuples consisting of a first name
and last name excludes pairs with a given initial letter of the last name and builds a string for the remaining ones,
gluing the first letter of the first name and last name into one in a row, inserting a dot between them:<br>
`findNames 'B'[("Adam", "Smith"),("Sue", "Brown"),("Tom", "Taylor")] == ["A.Smith", "T.Taylor"]`

<br><b>[task4](./task4.hs):</b><br>
    
Determine the type of polymorphic binary tree in which values are stored only in leaves, and implement the `sumLeafs`
function for counting the sum of values in the leaves of the tree.

<br><b>[task5](./task5.hs):</b><br>
    
Based on the known functionals, define the `isArithm` function, which determines whether the sequence of numbers
given by the list is an arithmetic progression:<br>
`isArithm [2, 7, 12, 17] == True`
