interleave :: [t] -> [t] -> [t]
interleave [] [] = []
interleave (x:xs) [] = x : interleave xs []
interleave [] (x:xs) = x : interleave [] xs
interleave (t0:t0s) (t1:t1s) = [t0] ++ [t1] ++ interleave t0s t1s
