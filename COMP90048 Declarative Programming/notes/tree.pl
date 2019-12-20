
%WS7 Q6
inset_member(N,tree(_,N,_)).
inset_member(N,tree(L,V,R)):-
	( N < V ->
		inset_member(N,L)
	; N > V,
		inset_member(N,R)
	).

% only update one side of the tree
inset_insert(N,empty,tree(empty,N,empty)).
inset_insert(N,tree(L,N,R),tree(L,N,R)).
inset_insert(N,tree(L,V,R),Result):-
  ( N < V ->
  	Result = tree(L1,V,R),
  	inset_insert(N,L,L1)
  ; N > V,
    Result = tree(L,V,R1),
    inset_insert(N,R,R1)
  ).


%WS8 Q2 tree_list

tree_list(empty,[]).
tree_list(node(L,N,R),List):-
	tree_list(L,List1),
	tree_list(R,List2),
	append(List1,[N|List2],List).

%W8 Q3 tree_list not append

tree_list1(Tree,List):- tree_list1(Tree,[],List).
tree_list1(empty,List,List).
tree_list1(node(L,N,R),List_in,List_out):-
	tree_list1(R,List_in,L1),
%	tree_list1(L,L1,List_out).
	tree_list1(L,[N|L1],List_out).


% W8 Q4 balanced tree

% how to write length, take 1 element of list as root
% half the number of tail as the length of Front,
% use append to concat Front and Back list.
list_tree([],empty).
list_tree([E|Tail],node(L,N,R)):-
	length(Tail,Len),
	Len2 = Len // 2,
	length(Front,Len2),
    append(Front,[N|Back],[E|Tail]),
	list_tree(Front,L),
	list_tree(Back,R).
	
% 14-2 Q5

intset_insert(N,empty,tree(empty,N,empty)).
intset_insert(N,tree(_,N,_),tree(_,N,_0)).
intset_insert(N,tree(L,V,R),Result):-
	( N < V ->
		Result = tree(L1,V,R),
		intset_insert(N,L,L1)
	; N > V,
	    Result = tree(L,V,R1),
	    intset_insert(N,R,R1)
	).




