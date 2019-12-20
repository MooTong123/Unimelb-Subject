% Author:  Moo Tong
% Date:  05/09/2019
% Email:	mtm@student.unimelb.edu.au
% ID:	1004452
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Purpose: This program is to solve the problem of Math Puzzles.
%
%	Example Puzzle               Solution of Puzzle
%           | 14 | 10 | 35 |                 | 14 | 10 |35 |
%   14 |    |    |    |              14 | 7  |  2 | 1 | 
%   15 |    |    |    |              15 | 3  |  7 | 5 |
%   28 |    |  1 |    |              28 | 4  |  1 | 7 |
%
% Constraints:
%	(1) Each row and each column contains no repeated digits(1-9);
%	(2) All squares on the diagonal line from upper left to lower 
%		right contain the same value;
%	(3) the heading of each row and column holds either the sum 
%		or the product of all the digits in that row or column.
%
% Implenatation Step:
%	(1) Make sure that all values in diagonal is the same
%	(2) Make sure that each row(except first row) should accept
%		all the constrains.
%	(3) Make sure that each column should accept all constains by
%		using transpose(library(clpfd)) to transpose column to row,
%		and then use the same methods as row's.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% library for Constraint Logic Programming, like transpose
:- use_module(library(clpfd)).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% puzzle_solution(?[Head|TailPuzzle]) 
% Purpose: To solve the Puzzle, and check three constains, which are
%		   vaild_diagonal, vaild_rows and vaild_columns.		

puzzle_solution([H|TP]) :-
	vaild_diagonal(TP),	
	vaild_rows(TP),
	vaild_columns([H|TP]).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% vaild_diagonal(?[Head|Tail])  
% Purpose:  To check the value of diagonal is the same, nth0 predicate
% 			is to get the diagonal value in the first row, and then check
%		   	whether the values in remaining rows are the same as the 
%			diagonal value in the first row.

vaild_diagonal([H|T]):-
	nth0(1,H,V),
	check_diagonal([H|T],1,V).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% check_diagonal(?[Head|Tail],?Index,?Value)
% Purpose: To check if the diagonol value in remaining rows are the same. 
%		   nth0 predicate is to get the diagonal value in this row.
%		   NextIndex is to get the next index of diagonal number.

check_diagonal([],_,_).
check_diagonal([H|T],I,V):-
	nth0(I,H,V),
	NI is I + 1,
	check_diagonal(T,NI,V).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% vaild_columns(+[Head|-TailPuzzle])
% Purpose: To check if the Puzzle has a vaild column. The method is to 
%		   transpose the orignal Puzzle, and then to check if the 
% 		   transpose rows are valid.

vaild_columns([]).
vaild_columns([H|TP]):-
	transpose([H|TP],[_|TTP]),
	vaild_rows(TTP).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% vaild_rows(+[?Head|?Tail])
% Purpose: To check if the rows are valid. The method is to check each row
%          one by one.

vaild_rows([]).
vaild_rows([H|T]):-
	vaild_one_row(H),
	vaild_rows(T).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% vaild_one_row(+[?Head|?Tail])
% Purpose: To solve one row of the Puzzle, and check if this row is 
%		   vaild. The method is to input the value, and then check if
%		   this row is suitable for the constain(3), finally check if
%		   the values in this row are all distinct.

vaild_one_row([]).
vaild_one_row([H|T]):-
	input_value(T),
	length(T,S),		
	check_sum_product(T,H,S),
	all_distinct(T).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% input_value(?[X|Xs])
% Purpose: To input the value in the row of Puzzle. The method is to check
%		   if the value in row is ground. If the value is already ground,
%		   then check remaining values. If the value is not ground, then
%		   input a value between 1 to 9.

input_value([]).
input_value([X|Xs]):-
	(   \+ ground(X)
    ->  between(1, 9, X),
        input_value(Xs)
    ;
        input_value(Xs)
    ).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% check_sum_product(?Tail,?Value,+Size)
% Purpose: To check if the row accepts the constain(3), which is the heading
%		   of the row is the sum or product of all remaining digits. The
%		   method is to check if the heading value is greater than the 
%		   maximum possible value of the sum firstly. If it does, then only 
%		   check the possibility of the product of all digits.

check_sum_product([],_).
check_sum_product(T,V,S):-
	( V > S * 9 
	->	product_of_row(T,V)
	;	(sum_of_row(T, V);
		product_of_row(T, V))
	).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% sum_of_row(+[Head|Tail], +Value)
% Purpose: To check if the sum of the list equals to the Value. 

sum_of_row([], 0).
sum_of_row([H|T], V) :-
  sum_of_row(T, V1),
  V #= V1 + H.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% product_of_row(+[Head|Tail], +Value)
% Purpose: To check if the product if all digits equals to the value.

product_of_row([], 1).
product_of_row([H|T], V) :-
  product_of_row(T, V1),
  V #= V1 * H.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%




