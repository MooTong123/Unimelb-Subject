-- Author: 	Moo Tong
-- Date:	19/03/2019
-- Email:	mtm@student.unimelb.edu.au
-- ID;		1004452

module Assignment1 (subst, interleave, unroll) where


--subst takes two values and a list
--and replace every occurrence of the first value with the sencond
subst :: Eq t => t -> t -> [t] -> [t]
subst _ _ [] = []
subst t0 t1 (x : xs) = 
    if t0 == x
    then t1 : subst t0 t1 xs
    else x : subst t0 t1 xs


--interleave takes two lists 
--and returns the interleaving of the two arguments
interleave :: [t] -> [t] -> [t]
interleave [] [] = []
interleave (x:xs) [] = x : interleave xs []
interleave [] (x:xs) = x : interleave [] xs
interleave (t0:t0s) (t1:t1s) = [t0] ++ [t1] ++ interleave t0s t1s



--unroll takes a list and an integer
--and the output consists of the input list repeated
--as many times as necessary to have the specifieed length
unroll :: Int -> [a] -> [a]
unroll _ [] = []

unroll a xs = 
    if a >= length xs then 
        xs ++ unroll (a - length xs) xs
    else if a < length xs && a > 0 then
        take a xs
    else [] -- if a < 0

