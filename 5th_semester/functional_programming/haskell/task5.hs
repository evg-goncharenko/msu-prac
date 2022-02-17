isArithm :: (Num a, Eq a) => [a] -> Bool
isArithm l = foldr (&&) True (zipWith (\f n -> n - f == p) l (tail l)) where p = (head (tail l) - (head l))

main = print(isArithm [-2, 0, 2, 4])
