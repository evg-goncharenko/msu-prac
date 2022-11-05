divide1([], [], []). 
divide1([X], [X], []). 
divide1([X,Y|T], [X|T1], [Y|T2]) :- divide1(T, T1, T2).
