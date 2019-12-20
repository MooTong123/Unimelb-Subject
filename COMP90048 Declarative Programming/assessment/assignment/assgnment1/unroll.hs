unroll :: Int -> [a] -> [a]
unroll _ [] = []

unroll a xs = 
    if a >= length xs then 
        xs ++ unroll (a - length xs) xs
    else if a < length xs && a > 0 then
        take a xs
    else [] -- if a < 0

