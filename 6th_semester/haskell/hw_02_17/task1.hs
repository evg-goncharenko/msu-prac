qsort :: [Int] -> [Int]
qsort [] = []
qsort (x:xs) = qsort (filter (< x) xs) ++ [x] ++ qsort (filter (>= x) xs)

main = print(qsort [58,12,60,7])
