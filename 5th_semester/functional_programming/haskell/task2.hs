data Tree = Empty | Node Int Tree Tree deriving (Show, Eq)
mulTrees :: Tree -> Tree -> Tree
mulTrees Empty Empty = Empty
mulTrees (Node a b c) Empty = Node a b c
mulTrees Empty (Node a b c) = Node a b c
mulTrees (Node a1 b1 c1) (Node a2 b2 c2) = Node (a1 * a2) (mulTrees b1 b2) (mulTrees c1 c2)

main = print(mulTrees (Node 18 Empty Empty) (Node 5 (Node 3 Empty Empty) Empty))
