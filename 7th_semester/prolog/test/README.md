## Prolog Test

*Read this in other languages: [English](README.md), [Русский](README.ru.md).*

<b>[task1](./task1.pl):</b><br>

Define the predicate `similar(L1, L2)` - lists `L1` and `L2` are similar.
Two lists are considered similar, in which the elements in the corresponding places
either coincide, or are any non-empty lists.
For example, similar to the following 2 lists:<br>
`[2, f(a), [4,c], -7, ssd, [3,x,2]]` и<br>
`[2, f(a), [a,c,r], -7, ssd, [9,w]]`

<br><b>[task2](./task2.pl):</b><br>

A binary tree is given, written as a term using the constant `empty`
(empty tree) and a ternary functor `tree`:<br>
*`tree` (node symbol label, left subtree, right subtree)*<br>

For example:
```
tree(g,tree(f,empty,empty),
    tree(k,tree(p,empty,tree(r,empty,empty)),
        empty))
```
Make a predicate `numLabel(T,V,N)`: `N` - is the number of times that the label `V`
occurs in the nodes of the tree `Т`.
```
numLabel(tree(g,tree(f,empty,empty),
    tree(f,tree(p,empty,tree(f,empty,empty)),empty)),f,3) => true
```

<br><b>[task3](./task3.pl):</b><br>

Make a predicate `eqElems(L1,L2,X)`: The same element `X` occurs in the lists `L1` and `L2`
at the same position from their beginning.
For example, when calculating:<br>
`eqElems([a,s,c,b,d],[z,s,c,a,d],X)`<br>
the elements `s`,`c`,`d` must be given in any order.

<br><b>[task4](./task4.pl):</b><br>
    
Make a predicate `divide(L1,L2,L3)` - the list `L1` is divided into lists `L2` and `L3`
approximately in half:
```
?- divide([a,b,c,d,e],[a,c,e],[b,d]). => true
```
