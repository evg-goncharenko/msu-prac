eqElems([],_, []):-!. 
eqElems(_,[], []):-!. 
eqElems([X1|T1], [X1|T2], [X1|T]) :- !, eqElems(T1, T2, T). 
eqElems([_|T1], [_|T2], T) :- eqElems(T1, T2, T).
