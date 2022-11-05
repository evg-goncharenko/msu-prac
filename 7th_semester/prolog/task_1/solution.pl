%11, 13, 16-20

% Вспомогательные функции с занятий.
no_doubles([], []).
no_doubles([X|T], T1) :- member(X, T), !, no_doubles(T, T1).
no_doubles([X|T], [X|T1]) :- no_doubles(T, T1).

per([], []).
per([X|T], P) :- per(T, T1), del_one(P, X, T1).

del_one([E|T], E, T).
del_one([X|T], E, [X|T1]) :- del_one(T, E, T1).

myunion([], M2, M2).
myunion([X|T], M2, M3) :- member(X, M2), !, myunion(T, M2, M3).
myunion([X|T], M2, [X|M3]) :- myunion(T, M2, M3).

% 11: union (М1, М2, М3): множество М3 - объединение множеств М1 и М2
% (i,i,o)
% (i,i,i)
union_11(M1, M2, M3) :- myunion(M1, M2, M), per(M, M3), !.


% 13: sub_tree (Т1, Т2): дерево Т1 является непустым поддеревом дерева Т2
% (i,i)
% (o,i)
% (i,o) - сработает, ответ недетерминированный
sub_tree(T, T) :- dif(T, empty).
sub_tree(T, tree(TL, TR, _)) :- sub_tree(T, TL) ; sub_tree(T, TR).


% 16: path (Х, У, L): L - путь без петель между вершинами Х и У, т.е.
% список вершин между этими вершинами.
% (i,i,o) - недетерминированный
% (o,i,i) - сработает, ответ недетерминированный
% (i,o,i) - сработает, ответ недетерминированный
% (o,o,i) - сработает, ответ недетерминированный
% (i,i,i) - сработает, ответ недетерминированный

% Граф (рис. 2):
edge(a, c, 8).
edge(a, b, 3).
edge(c, g, 1).
edge(g, d, 12).
edge(c, f, 1).
edge(f, h, 2).
edge(h, c, 4).
edge(b, d, 0).
edge(e, d, 9).

% Вспомогательная функция: mypath(X, Y, L, ACC, C) (с накапливающим параметром).
mypath(X, Y, L, ACC, C) :-
    (edge(X, Y, C); edge(Y, X, C)), append(ACC, [X, Y], L), not(member(Y, ACC)).
mypath(X, Y, LRES, ACC, CRES) :-
    (edge(X, Z, C); edge(Z, X, C)), not(member(Z, ACC)), append(ACC, [X], L),
    mypath(Z, Y, LRES, L, COUNT), CRES is COUNT + C.

path(X, X, [X]) :- !.
path(X, Y, L) :- mypath(X, Y, L, [], _).


% 17: min_path (Х, У, L): L - путь между вершинами Х и У, имеющий
% минимальную стоимость
% (i,i,o) - недетерминированный
% (i,i,i) - сработает, ответ недетерминированный

% Вспомогательная функция: минимум из списка путей по стоимости.
findmin([[_,C]], C) :- !.
findmin([[_,C]|T], MIN) :- findmin(T, C1), MIN is min(C, C1).

findmin([], _, _) :- fail, !.
findmin([[P,C]|_], MIN, P) :- MIN =:= C.
findmin([[_,_]|T], MIN, P) :- findmin(T, MIN, P).

min_path(X, Y, L) :-
    findall([P, C], mypath(X, Y, P, [], C), RES), findmin(RES, MIN), findmin(RES, MIN, L).


% 18: short_path (Х, У, L): L - самый короткий путь между вершинами Х и
% У (по количеству ребер).
% (i,i,o) - недетерменированный
% (i,i,i) - сработает, ответ недетерминированный

% Вспомогательная функция: находит все следующие вершины списка вершин.
find_next([], _, []).
find_next([X|T], MARK, RES) :-
    findall(Z,((edge(X, Z, _); edge(Z, X, _)), not(member(Z, MARK))), NEXT),
    find_next(T, MARK, L), append(L, NEXT, NEXT1), no_doubles(NEXT1, RES).

% Вспомогательная функция: кратчайший путь и его длина.
path_w([], _, _, _, _) :- !, fail.
path_w(STEP, Y, MARK, [X,Y], 1):-
    find_next(STEP, MARK, NEXT), member(Y, NEXT), !,
    (edge(X, Y, _); edge(Y, X, _)), member(X, STEP).
path_w(STEP, Y, MARK, [X,H|T], LEN) :-
    find_next(STEP, MARK, NEXT), append(MARK, STEP, L), no_doubles(L, NEWM),
    path_w(NEXT, Y, NEWM, [H|T], N), LEN is N+1,
    (edge(X, H, _); edge(H, X, _)), member(X, STEP).

short_path(X, Y, L) :- path_w([X], Y, [], L, _).


% 19: cyclic: граф является циклическим (т.е. не является деревом).
cyclic :- edge(X, Y, _), findall(PATH, path(X, Y, PATH), RES), length(RES, L), \+ L = 1, !.


% 20: is_connected: граф является связным.

% Вспомогательная функция: список всеx вершин.
edges(RES) :- findall(X, (edge(X, _, _); edge(_, X, _)), L), no_doubles(L, RES).

% Вспомогательная функция: существуют ли пути из X в другие.
is_connected(_, []) :- !.
is_connected(X, [Y|T]) :- path(X, Y, _), !, is_connected(X, T).

is_connected :- edges([X|T]), is_connected(X,T) .


