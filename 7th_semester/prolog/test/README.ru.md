## Контрольная по языку Пролог

*Read this in other languages: [English](README.md), [Русский](README.ru.md).*

<b>[task1](./task1.pl):</b><br>

Определить предикат `similar(L1, L2)` - списки `L1` и `L2` подобны.
Подобными считаются два списка, в которых элементы на соответствующих местах или
совпадают, или являются любыми непустыми списками.
Например, подобны следующие 2 списка:<br>
`[2, f(a), [4,c], -7, ssd, [3,x,2]]` и<br>
`[2, f(a), [a,c,r], -7, ssd, [9,w]]`

<br><b>[task2](./task2.pl):</b><br>

Дано бинарное дерево, записанное в виде терма с помощью константы `empty`
(пустое дерево) и тернарного функтора `tree`:<br>
*`tree` (метка-символ узла, левое поддерево, правое поддерево)*<br>

Например:
```
tree(g,tree(f,empty,empty),
    tree(k,tree(p,empty,tree(r,empty,empty)),
        empty))
```
Составить предикат `numLabel(T,V,N)`: `N` - количество раз, которое метка `V`
встречается в узлах дерева `Т`.
```
numLabel(tree(g,tree(f,empty,empty),
    tree(f,tree(p,empty,tree(f,empty,empty)),empty)),f,3) => true
```

<br><b>[task3](./task3.pl):</b><br>

Составить предикат `eqElems(L1,L2,X)`: в списках `L1` и `L2` на одинаковой
позиции от их начала встречается один и тот же элемент `Х`. Например, при
вычислении:<br>
`eqElems([a,s,c,b,d],[z,s,c,a,d],X)`<br>
должны быть выданы элементы `s`,`c`,`d` в любом порядке.

<br><b>[task4](./task4.pl):</b><br>
    
Составить предикат `divide(L1,L2,L3)` - список `L1` делится на списки `L2` и `L3`
примерно пополам:
```
?- divide([a,b,c,d,e],[a,c,e],[b,d]). => true
```