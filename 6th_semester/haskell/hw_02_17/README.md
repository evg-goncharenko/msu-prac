## Homework by 02.17

*Read this in other languages: [English](README.md), [Русский](README.ru.md).*

<b>[task1](./task1.hs):</b><br>

Sort a list of integers using a quick sort algorithm:

```haskell
qsort :: [Int] -> [Int]
qsort [1,10,2,15] => [1,2,10,15]
```

<br><b>[task2](./task2.hs):</b><br>

Convert each word in the word list so that it starts with a capital letter. Assume that `toUpper` function
is already defined (Data.Char module).

```haskell
capitalize :: [String] -> [String]
capitalize ["hello", "darkness", "my", "old", "firend"] => ["Hello","Darkness","My","Old","Firend"]
```
