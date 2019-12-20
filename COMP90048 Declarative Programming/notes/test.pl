
%%%%%%%%%%%%%%%%%%%%%%%%%%%

len([],0).
len([_|T],N):- len(T,X),N is X + 1.


leng(List,Length):- accLen(List,0,Length).

accLen([_|T],A,L):- Anew is A + 1, accLen(T,Anew,L).
accLen([],A,A).
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

accMax(List,Max):- 
	List = [H|_],
	accMax1(List,H,Max).

accMax1([H|T], A, Max) :- H > A, accMax1(T, H, Max).
accMax1([H|T], A, Max) :- H =< A, accMax1(T, A, Max).
accMax1([], A, A).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%input: [H|T] + L2
%result: [H| T + L2]
append1([],L,L).
append1([H|T1],L2,[H|T3]):- append(T1,L2,T3).

%ppt version
append2([],C,C).
append2([A|B],C,[A|BC]):- append2(B,C,BC).


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

reverse3(ABC,CBA):-
	sameLength(ABC,CBA),
	reverse1(ABC,CBA).

reverse1([],[]).
reverse1([A|BC],CBA):-
	reverse1(BC,CB),
	append(CB,[A],CBA).

sameLength([],[]).
sameLength([_|Xs],[_|Ys]):-
	sameLength(Xs,Ys).


reverseNew(List,Rev):- accRev(List,[],Rev).
accRev([H|T],A,R):- accRev(T,[H|A],R).
accRev([],A,A).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

prefix(P,L):- append(P,_,L).
suffix(S,L):- append(_,S,L).

subList(SubL,L):- suffix(S,L),prefix(SubL,S).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

proper_list([]).
proper_list([_Head|Tail]):-
	proper_list(Tail).
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% length([1,2,3],Len).

take(N,List,Front):-
	length(Front,N),
	append(Front,_,List).
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

member1(Elt,List):- append(_,[Elt|_],List).

member2(Elt,[Elt|_]).
member2(Elt,[_|Tail]):- member2(Elt,Tail).
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% X = 6 * 7 =====> X = 6 * 7
% X is 6 * 7 ====> X = 42

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% right side should be initialized.
sqaure(N,N2) :- N2 is N * N.

% sqare(5,X). =====>  25
% squre(X,25). ======> No

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


fact(N,F) :-
	(N =:= 0 ->
		F = 1
	;   N > 0,
		N1 is N -1,
		fact(N1,F1),
		F is F1 * N
	).

fact1(N,F):- fact2(N,1,F).
fact2(N,A,F):-
	(N =:= 0 ->
		F = A
	;	N > 0,
		N1 is N - 1,
		A1 is A * N,
		fact2(N1,A1,F)
	).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

map(_,[],[]).
map(P,[X|Xs],[Y|Ys]):-
	call(P,X,Y),
	map(P,Xs,Ys).
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


lnf([],[]).
lnf([X|Xs],Ys) :- X = 5, lnf(Xs,Ys).
lnf([X|Xs],[X|Zs]) :- X \= 5, lnf(Xs,Zs).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% member in this tree
%inset_member(+N,+Set).
inset_member(N,tree(_,N,_)).
inset_member(N,tree(L,N0,_)):-
	N < N0, inset_member(N,L).
inset_member(N,tree(_,N0,R)):-
	N > N0, inset_member(N,R).


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


listof(_,[]).
listof(Ele,[Ele|Tail]):-
	listof(Ele,Tail).
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

allsame(List):- listof(_,List).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

adjacent(E1,E2,List):-
	append(_,[E1,E2|_],List).

adjacent2(E1,E2,[E1,E2|_]).
adjacent2(E1,E2,[_|Tail]):-
	adjacent2(E1,E2,Tail).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


before(E1,E2,[E1|Tail]):-
	member3(E2,Tail).
before(E1,E2,[_|Tail]):-
	before(E1,E2,Tail).

member3(E1,[E1|_]).
member3(E1,[_|Tail]):-
	member3(E1,Tail).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%workshop 7

inset_member1(N,tree(_,N,_)).
inset_member1(N,tree(L,V,R)):-
	( N < V ->
	  inset_member1(N,L)
	; inset_member1(N,R)
	).

intset_insert1(N,empty,tree(empty,N,empty)).
intset_insert1(N,tree(L,N,R),tree(L,N,R)).
intset_insert1(N,tree(L,V,R),Result):-
	( N < V ->
	  Result = tree(L1,V,R),
	  intset_insert1(N,L,L1) 
	; N > V,
	  Result = tree(L,V,R1),
	  intset_insert1(N,R,R1)
	).
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


sumlist1(List,Sum):-sumlist1(List,0,Sum).

sumlist1([],Sum,Sum).
sumlist1([N|Ns],Acc,Sum):-
	Acc1 is Acc + N,
	sumlist1(Ns,Acc1,Sum).


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

tree_list(empty,[]).
tree_list(node(L,E,R),List):-
	tree_list(L,List1),
	tree_list(R,List2),
	append(List1,[E|List2],List).

% first R, then L, [2,3,4]
% first L, then R, [4,3,2]
tree_list1(Tree,List):- tree_list1(Tree,[],List).
tree_list1(empty,List,List).
tree_list1(node(L,E,R),List_in,List_out):-
	tree_list1(R,List_in,L1),
	tree_list1(L,[E|L1],List_out).


% balanced tree

list_tree([],empty).
list_tree([Ele|List],node(L,E,R)):-
	length(List,Len),
	Len1 is Len // 2,
	length(Front,Len1),
	append(Front,[E|Back],[Ele|List]),
	list_tree(Front,L),
	list_tree(Back,R).



divi(A,B,C):- C is A // B. 

%%   (5,2,X)   X = 2
%%   (7,2,X)   X = 3


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

same_elements(L1,L2):-
	all_in(L1,L2),
	all_in(L2,L1).

all_in([],_).
all_in([E|Es],L):-
	member(E,L),
	all_in(Es,L).


same_elements1(L1,L2):-
	sort(L1,Sorted),
	sort(L2,Sorted).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

even_list(List) :- length(List, Len), 
	Len mod 2 =:= 0.



