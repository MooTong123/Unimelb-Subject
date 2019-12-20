%correspond:

correspond(E1,[E1|_],E2,[E2|_]).
correspond(E1,[_Head1|Tail1],E2,[_Head2|Tail2]) :-
    correspond(E1,Tail1,E2,Tail2). %Continue to find the match in remaining tails.

%interleave:

append([],C,C).
append([A|B],C,[A|BC]):- append(B,C,BC).

interleave(Ls,[]):- 
	empty(Ls). %Proper list.
interleave(Ls,L):-
	firstElem(Ls,Heads), %Operate on all first elements of the lists and store in Heads.
	lastElems(Ls,Tails), %Operate on remaining elements and store in Tails.
	interleave(Tails,Tail_L), %Use Tails as a new Ls and store results of every time in L.
	append(Heads,Tail_L,L).

firstElem([],[]).
firstElem(Ls,Heads):-
	Ls = [Head_list|Tail_lists],
	Head_list = [_Head|_Tail],
	firstElem(Tail_lists,Hs),
	Heads = [_Head|Hs].

lastElems([],[]).
lastElems(Ls,Tails):-
	Ls = [Head_list|Tail_lists],
	Head_list = [_Head|_Tail],
	lastElems(Tail_lists,Ts),
	Tails = [_Tail|Ts].

empty([]).
empty(Ls):-
	Ls=[[]|Tail_lists],
	empty(Tail_lists).

%partial_eval:

partial_eval(Expr0,Var,Val,Expr):-
	Expr0=..List1, %Split Expr0 into elements and store in List1.
	replace_expr(List1,Var,Val,[],Expr,0,_,0,_).

replace_expr([],_,_,List,Expr,Num,Num1,Atom,Atom1):- 
	reverse(List,List1), %Appended list is reverse.
	Expr0 =.. List1,
					   %If number is one more than atom(symbols or not Var)
					   %then the expr should be calculated.
					   %Else keep it the same.

	Num1 is Num,	   %Store Num1 for sub_expr.
	Atom1 is Atom,	   %Store Atom1 for sub_expr.
	(Num1 =:= Atom1 + 1 ->  
		Expr is Expr0
	;\+ (Num1 =:= Atom1 + 1) ->
		Expr = Expr0
	).


replace_expr([Head|Tails],Var,Val,Oldlist,Expr,Num,Num1,Atom,Atom1):-
	(number(Head) -> %The element is a number.
	append([Head],Oldlist,Newlist), %Put the element in the new list.
	New_num is Num + 1,
	replace_expr(Tails,Var,Val,Newlist,Expr,New_num,Num1,Atom,Atom1)
	;atom(Head),Head = Var-> %The element is an atom and = var.
	append([Val],Oldlist,Newlist), %Put val in the new list.
	New_num is Num + 1,
	replace_expr(Tails,Var,Val,Newlist,Expr,New_num,Num1,Atom,Atom1)
	;atom(Head),\+ (Head = Var) -> %The element is an atom and != var.
	append([Head],Oldlist,Newlist), %Put the element in the new list.
	New_atom is Atom + 1,
	replace_expr(Tails,Var,Val,Newlist,Expr,Num,Num1,New_atom,Atom1)
	;\+ number(Head),\+ atom(Head),\+ (Head =Var) -> %The element is not a number 
													 %or an atom 
													 %or = var. (Maybe symbols or expressions)
	Head =..Sub_expr0,
	replace_expr(Sub_expr0,Var,Val,[],Sub_expr,0,Sub_num,0,Sub_atom), %Store the Sub_num and Sub_atom.
	New_num is Num + Sub_num,
	New_atom is Atom + Sub_atom,
	append([Sub_expr],Oldlist,Newlist),
	replace_expr(Tails,Var,Val,Newlist,Expr,New_num,Num1,New_atom,Atom1)
	).