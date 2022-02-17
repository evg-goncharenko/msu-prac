data BiTree a = Leaf a | BiTree (BiTree a) (BiTree a) deriving Show
sumLeafs :: (Num a) => BiTree a -> a 
sumLeafs (Leaf a) = a 
sumLeafs (BiTree left right) = (sumLeafs left) + (sumLeafs right)

main = print(sumLeafs (BiTree (Leaf 1) (BiTree (Leaf 2)(Leaf 3))))
