similar([],[]).
similar([X|T1], [X|T2]):- !, similar(T1, T2).
similar([X|T1], [Y|T2]):- is_list(X), is_list(Y), similar(T1, T2).
