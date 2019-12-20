subst :: Eq t => t -> t -> [t] -> [t]
subst _ _ [] = []
subst t0 t1 (x : xs) = 
	if t0 == x
	then t1 : subst t0 t1 xs
	else x : subst t0 t1 xs