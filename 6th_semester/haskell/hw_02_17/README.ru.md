## Домашнее задание к 02.17

*Read this in other languages: [English](README.md), [Русский](README.ru.md).*

<b>[task1](./task1.hs):</b><br>

Отсортировать список целых чисел алгоритмом быстрой сортировки:

```haskell
qsort :: [Int] -> [Int]
qsort [1,10,2,15] => [1,2,10,15]
```

<br><b>[task2](./task2.hs):</b><br>

Преобразовать каждое слово в списке слов так, чтобы оно начиналось с заглавной буквы. Считать, что функция `toUpper`
уже определена (модуль Data.Char).

```haskell
capitalize :: [String] -> [String]
capitalize ["hello", "darkness", "my", "old", "firend"] => ["Hello","Darkness","My","Old","Firend"]
```
