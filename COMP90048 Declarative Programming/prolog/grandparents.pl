

grandparent(X,Y) :- parent(X,Z),child(Y,Z).

sibling(Sib1,Sib2) :- ]
		parent(X,Sib1),parent(X,Sib2), Sib1 /= Sib2. 




