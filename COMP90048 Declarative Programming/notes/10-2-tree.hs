data Ttree t = Nil | Node3 t (Ttree t) (Ttree t) (Ttree t)
get_result :: Ord a => Ttree a -> ([a],[[a]])
get_result Nil = ([],[])
get_result t@(Node3 a _ _ _) =
    let con = return_lists t
        shortest = (snd.head.sort.get_shortest.tail) con
    in 
        (shortest, con)

get_shortest :: [[a]] -> [(Int,[a])]
get_shortest [] = []
get_shortest (x:xs) = let len = length x
       in (len,x):get_shortest xs

return_lists :: Ttree a -> [[a]]
return_lists Nil = [[], [], []]
return_lists (Node3 t left mid right) =
    let left_result  = get_lists left
        mid_result   = get_lists mid
        right_result = get_lists right
    in 
        [[t],left_result, mid_result, right_result]

get_lists :: Ttree a -> [a]
get_lists Nil = []
get_lists (Node3 t Nil Nil Nil) = [t]
get_lists (Node3 t left mid right) = t :(get_lists left ++ get_lists mid ++ get_lists right)