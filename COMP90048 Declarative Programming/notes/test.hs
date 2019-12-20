import System.IO.Unsafe

--myList (x:xs) = myList xs
--myList [x] = x
myList (x:xs) = myList xs

getTwos :: [[a]] -> [[a]]
getTwos = filter  ((==2) . length)


xor1 False False = False
xor1 False True  = True
xor1 True  False = True
xor1 True  True  = False

xor2 x y = (x || y) && not (x && y)


myHead [] = error "empty list"
myHead (x:_) = x

myTail [] = error "empty list"
myTail (_:xs) = xs


append [] lst = lst
append (e:es) lst = e: append es lst

myReverse [] = []
myReverse (x:xs) = myReverse xs ++ [x]

getNthElem :: Int -> [a] -> a
getNthElem n (x:xs)
    | n < 0 = error "error"
    | n == 0 = x
    | otherwise = getNthElem (n-1) xs

data Suit =  Club | Diamond | Heart | Spade
--             deriving Show
data Rank = R2 | R3 | R4 | R5 | R6 | R7 | R8
            | R9 | R10 | Jack | Queen | King | Ace
 --           deriving Show
data Card = Card Suit Rank 
--    deriving Show

instance Show Card where show = showCard
instance Show Suit where show = showSuit
instance Show Rank where show = showRank


showSuit :: Suit -> String
showSuit Club = "Club"

showRank :: Rank -> String
showRank R2 = "R2"

showCard :: Card -> String
showCard (Card s r) = showSuit s ++ " " ++ showRank r



data FontColor 
        = Color_name String
        | Hex Int
        | RGB (Int,Int,Int)
data FontTag = FontTag (Maybe Int) (Maybe String)
                 (Maybe FontColor)

factorial :: Int -> Int
factorial 0 = 1
factorial n = n * (factorial (n-1))


myElem :: Eq a => a -> [a] -> Bool
myElem _ [] = False
myElem n (x:xs)
      | n == x = True
      | otherwise = myElem n xs

logestPrefix :: Eq a => [a] -> [a] -> [a]
logestPrefix [] _ = []
logestPrefix _ [] = []
logestPrefix (x:xs) (y:ys) 
           | x == y = x: logestPrefix xs ys
           |otherwise = []


mccarthy_91 :: Int -> Int 
mccarthy_91 n = myWhile 1 n

myWhile :: Int -> Int -> Int
myWhile c n  
     | c /= 0 = nextWhile c n
     | otherwise = n
        where nextWhile c n | n > 100 =  myWhile (c-1) (n-10) 
                            | otherwise = myWhile (c+1) (n+11)


range_list :: Int -> Int -> [Int]
range_list min max = 
    if min > max then []
    else min : range_list (min + 1) max

--ftoc :: Double -> Double
ftoc :: Fractional a => a -> a
ftoc f = (5/9) * (f - 32)

quadRoots :: (Floating a, Ord a) => a -> a -> a -> [a]
quadRoots 0 0 _ = error "no solution"
quadRoots 0 b c = [-c / b]
quadRoots a b c 
    | con == 0 = [root]
    | con > 0 =  [root + temp, root - temp]
    | otherwise = error "no solution"
    where con = b * b - 4 * a * c
          root =  b / (-2 * a)
          temp = sqrt con / (2 * a)


merge :: Ord a => [a] -> [a] -> [a]
merge [] ys = ys
merge xs [] = xs
merge (x:xs) (y:ys)
      | x <= y = x: merge xs (y:ys)
      | x > y = y : merge (x:xs) ys

qsort1 :: Ord a => [a] -> [a]
qsort1 [] = []
qsort1 (pivot:xs) = qsort1 lessor ++ [pivot] ++ qsort1 larger
    where lessor = filter (<= pivot) xs
          larger = filter (> pivot) xs

data Tree k v = Leaf | Node1 k v (Tree k v) (Tree k v)
     deriving(Eq,Show)
same_shape :: Tree a b -> Tree c d -> Bool
same_shape Leaf Leaf = True
same_shape Leaf (Node1 _ _ _ _) = False
same_shape (Node1 _ _ _ _) Leaf = False
same_shape (Node1 _ _ l1 r1) (Node1 _ _ l2 r2)
     = (same_shape l1 l2) && (same_shape r1 r2)

data Expression
       = Var Variable
       | Num Integer
       | Plus Expression Expression
       | Minus Expression Expression
       | Times Expression Expression
       | Div Expression Expression
data Variable = A | B
eval :: Integer -> Integer -> Expression -> Integer
eval a _ (Var A) = a
eval _ b (Var B) = b
eval _ _ (Num n) = n
eval a b (Plus e1 e2) = (eval a b e1) + (eval a b e2)
eval a b (Minus e1 e2) = (eval a b e1) - (eval a b e2)
eval a b (Times e1 e2) = (eval a b e1) * (eval a b e2)
eval a b (Div   e1 e2) = (eval a b e1) `div` (eval a b e2)




data Tree1 a = Empty | Node (Tree1 a) a (Tree1 a)

treesort :: Ord a => [a] -> [a]
treesort xs = tree_inorder (list_to_bst xs)

list_to_bst:: Ord a => [a] -> Tree1 a
list_to_bst [] = Empty
list_to_bst (x:xs) = bst_insert x (list_to_bst xs)

bst_insert :: Ord a => a -> Tree1 a -> Tree1 a
bst_insert i Empty = Node Empty i Empty
bst_insert i (Node l v r) 
     | i <= v = Node (bst_insert i l) v r
     | i > v = Node l v (bst_insert i r) 

tree_inorder :: Tree1 a -> [a]
tree_inorder Empty = []
tree_inorder (Node l v r) = tree_inorder l ++ [v] 
         ++ tree_inorder r



transpose1 :: [[a]] -> [[a]]
transpose1 [] = error "error"
transpose1 list@(xs:xss)
    | len > 0 = transpose1' len list
    | otherwise = error "error"
    where len = length xs

transpose1' len [] = replicate len []
transpose1' len (xs:xss)
  | len == length xs = zipWith (:) xs (transpose1' len xss)
  | otherwise = error "error"


status1 ns = (length ns, sum ns, sumsq ns)

sumsq [] = 0
sumsq (n:ns) = n*n + sumsq ns

status2 [] = (0,0,0)
status2 (n:ns) 
   = let (l,s,sq) = status2 ns
   in (l+1,s+n,sq+n*n)

maybeApply :: (a->b) -> Maybe a -> Maybe b
maybeApply f Nothing = Nothing
maybeApply f (Just x) = Just (f x)


zWith :: (a->b->c) -> [a] ->[b]->[c]
zWith f [] _ = []
zWith f _ [] = []
zWith f (x:xs) (y:ys) = f x y: zWith f xs ys



linearEqn :: Num a => a -> a -> [a] -> [a]
linearEqn m n = map (\x -> m*x + n)



sqrtPM :: (Floating a, Ord a) => a -> [a]
sqrtPM x
  | x  > 0    = let y = sqrt x in [y, -y] 
  | x == 0    = [0]
  | otherwise = []

allSqrts :: (Floating a, Ord a) => [a] -> [a]
allSqrts xs = foldl (++) [] (map sqrtPM xs)
allSqrts1 xs = concat (map sqrtPM xs)
allSqrts2 xs = concatMap sqrtPM xs

sqrt_pos1 :: (Ord a, Floating a) => [a] -> [a]
sqrt_pos1 ns = map sqrt (filter (>=0) ns)

sqrt_pos2 :: (Ord a, Floating a) => [a] -> [a]
sqrt_pos2 (n:ns) = if n >= 0 then sqrt n : sqrt_pos2 ns
                   else sqrt_pos2 ns


hello :: IO ()
hello = 
    putStr "Hello, "
    >>= 
    \_ -> putStrLn "world!"


hello1 = do
  putStr "Hello, "
  putStrLn "world"


greet :: IO ()
greet = 
  putStr "Greetings! What is your name?"
  >>= 
  \_ -> getLine
  >>= 
  \name -> (
     putStr "Where are you from?"
     >>= 
     \_ -> getLine
     >>=
     \town ->
      let msg = "Welcome, " ++ name ++ " from " ++ town
      in putStrLn msg
    )

greet1 :: IO ()
greet1 = do
  putStr "Greerings! What is your name? "
  name <- getLine
  putStr "Where are you from? "
  town <- getLine
  let msg = "Wecome, " ++ name ++ " from " ++ town
  putStrLn msg
  putStrLn ("Welcome2, " ++ name ++ " from " ++ town )


main :: IO ()
main = do
  putStrLn "Please input a string"
  len <- readlen
  putStrLn $ "The length of that string is " ++ show len

readlen :: IO Int
readlen = do
  str <- getLine
  return (length str)



main1 :: IO ()
main1 = do
  putStrLn "Table of squares:"
  print_table 1 10

print_table :: Int -> Int -> IO ()
print_table cur max
  | cur > max = return ()
  | otherwise = do
     putStrLn (table_entry cur)
     print_table (cur+1) max

table_entry :: Int -> String
table_entry n = (show n) ++ " ^2 = " ++ (show (n*n))


sum1 :: Int -> Int -> Int
sum1 x y = unsafePerformIO $ do
  putStrLn ("summing " ++ (show x) ++ " and " ++ (show y))
  return (x + y)



maybeDec :: Integer -> Maybe Integer
maybeDec n = if n > 0 then Just (n-1) else Nothing



















