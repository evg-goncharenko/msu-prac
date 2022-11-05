numLabels(empty, _, 0). 
numLabels(tree(V, L, R), V, N) :- !, numLabels(L, V, N1), 
numLabels(R, V, N2), 
N is N1 + N2 + 1. 
numLabels(tree(_, L, R), V1, N) :- numLabels(L, V1, N1), 
numLabels(R, V1, N2), 
N is N1 + N2.
