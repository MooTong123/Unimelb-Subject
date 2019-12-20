
% Author: 	Moo Tong
% Date:		25/04/2019
% Email:	mtm@student.unimelb.edu.au
% ID;		1004452


% correspond(E1,L1,E2,L2)
correspond(E1,[E1|_],E2,[E2|_]).
correspond(E1,[_|L1],E2,[_|L2]):-
	correspond(E1,L1,E2,L2).


% interleave(Ls,L)

interleave(Ls,[]):-
	properList(Ls).
interleave(Ls,L):-
	first(Ls,Firsts),
	tail(Ls,Tails),
	append(Firsts,List,L),
	interleave(Tails,List).
	

first([],[]).
first([[FirstEle|_]|Tail],Firsts):-
	first(Tail,Fs),
	Firsts = [FirstEle|Fs].

tail([],[]).
tail([[_|RemELes]|Tail],Tails):-
	tail(Tail,Ts),
	Tails = [RemELes|Ts].

properList([]).
properList([[]|Tails]):-
	properList(Tails).


% partial_eval(Expr0,Var,Val,Expr)

% if Expr0 is a number
partial_eval(X,_,_,X):-
	number(X).

% if Expr0 is an atom
partial_eval(X,Y,Val,Expr):-
	atom(X),
	( X = Y ->
		Expr = Val
	;	Expr = X
	).

% if Expr0 is a term of x+y
partial_eval(X+Y,Z,Val,Expr):-
	partial_eval(X,Z,Val,Expr1),
	partial_eval(Y,Z,Val,Expr2),
	( number(Expr1),number(Expr2) ->
		Expr is Expr1 + Expr2
	;	Expr = Expr1 + Expr2
	).


% if Expr0 is a term of x-y
partial_eval(X-Y,Z,Val,Expr):-
	partial_eval(X,Z,Val,Expr1),
	partial_eval(Y,Z,Val,Expr2),
	( number(Expr1),number(Expr2) ->
		Expr is Expr1 - Expr2
	;	Expr = Expr1 - Expr2
	).

% if Expr0 is a term of x*y
partial_eval(X*Y,Z,Val,Expr):-
	partial_eval(X,Z,Val,Expr1),
	partial_eval(Y,Z,Val,Expr2),
	( number(Expr1),number(Expr2) ->
		Expr is Expr1 * Expr2
	;	Expr = Expr1 * Expr2
	).

% if Expr0 is a term of x/y
partial_eval(X/Y,Z,Val,Expr):-
	partial_eval(X,Z,Val,Expr1),
	partial_eval(Y,Z,Val,Expr2),
	( number(Expr1),number(Expr2) ->
		Expr is Expr1 / Expr2
	;	Expr = Expr1 / Expr2
	).

% if Expr0 is a term of x//y
partial_eval(X//Y,Z,Val,Expr):-
	partial_eval(X,Z,Val,Expr1),
	partial_eval(Y,Z,Val,Expr2),
	( number(Expr1),number(Expr2) ->
		Expr is Expr1 // Expr2
	;	Expr = Expr1 // Expr2
	).


