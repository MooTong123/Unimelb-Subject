-- workshop3 Q4 merge two sorted lists

mergelist :: Ord a => [a] -> [a] ->[a]
mergelist xs [] =xs
mergelist [] ys = ys 
mergelist (x:xs) (y:ys) 
   | x < y = x : mergelist xs (y:ys)
   | otherwise = y : mergelist (x:xs) ys



-- WS3 Q5 quick sort

qsort :: Ord a => [a] -> [a]
qsort [] = []
qsort (pivot:tail) = qsort lessor ++ [pivot] ++ qsort larger
     where lessor = filter (< pivot) tail
           larger = filter (>= pivot) tail 

-- WS3 Q6  数比较形状

data Tree k v = Leaf | Node k v (Tree k v) (Tree k v)

same_shape :: Tree a b -> Tree c d -> Bool
same_shape Leaf Leaf = True
same_shape Leaf (Node _ _ _ _) = False
same_shape (Node _ _ _ _) Leaf = False
same_shape (Node _ _ l1 r1) (Node _ _ l2 r2) =
  (same_shape l1 l2) && (same_shape r1 r2)


-- WS4 Q1   inset tree   inorder

data Tree1 a = Empty | Node1 (Tree1 a) a (Tree1 a)

treesort :: Ord a => [a] -> [a]
treesort list = inorder (set_into_bst (list))

set_into_bst :: Ord a => [a] -> Tree1 a
--wrong
set_into_bst [] = Empty 
set_into_bst (x:xs) = inset_insert x (set_into_bst xs)

inset_insert :: Ord a => a -> Tree1 a -> Tree1 a
inset_insert n Empty = Node1 Empty n Empty
inset_insert n (Node1 l v r)
-- worng
  | n < v = Node1 (inset_insert n l) v r
  | otherwise = Node1 l v (inset_insert n r)

inorder :: Ord a => Tree1 a -> [a]
inorder Empty  = []
inorder (Node1 l v r) = inorder l ++ [v] ++ inorder r



--Ws extra Q10  map_tree

data Tree2 a = Empty2 | Node2 (Tree2 a) a (Tree2 a)

map_tree :: (a->a) -> Tree2 a -> Tree2 a
map_tree _ Empty2 = Empty2
map_tree f (Node2 l v r)
  = Node2 (map_tree f l) (f v) (map_tree f r)


-- Q11 foldr_tree 

foldr_tree :: (a->b->a->a) -> a -> Tree2 b -> a
foldr_tree f b Empty2 = b
foldr_tree f b (Node2 l n r)
  = f (foldr_tree f b l) n (foldr_tree f b r)

height_tree = foldr_tree (node_max_plus_1) 0
  where node_max_plus_1 l _ r = 1 + (max l r)

size_tree = foldr_tree node_plus_1 0
  where node_plus_1 l _ r = 1 + l + r

sum_tree = foldr_tree (ternary (+)) 0
product_tree = foldr_tree (ternary (*)) 1
concat_tree = foldr_tree (ternary (++)) []

ternary :: (a -> a -> a) -> a -> a -> a -> a
ternary f x y z = f x (f y z)

max_tree :: Ord a => Tree2 a -> Maybe a
max_tree = foldr_tree node_max Nothing
  where node_max l n r = maybe_max (maybe_max l (Just n)) r

maybe_max :: (Ord a) => Maybe a -> Maybe a -> Maybe a
maybe_max Nothing Nothing = Nothing
maybe_max (Just x) Nothing = (Just x)
maybe_max Nothing (Just x) = (Just x)
maybe_max (Just x) (Just y) = Just (max x y)


--Q15

data Ltree a = LTLeaf a | LTBranch (Ltree a) (Ltree a)
data Cord a = Nil | Leaf2 a | Branch (Cord a) (Cord a)
data Mtree a = Mnode a [Mtree a]


--answer :map

ltree_map :: (a->b) -> Ltree a -> Ltree b
ltree_map f (LTLeaf a) = LTLeaf (f a)
ltree_map f (LTBranch l r) 
  = LTBranch (ltree_map f l) (ltree_map f r)

cord_map :: (a->b)-> Cord a -> Cord b
cord_map f Nil = Nil
cord_map f (Leaf2 a) = Leaf2 (f a)
cord_map f (Branch l r) 
  = Branch (cord_map f l) (cord_map f r)


mtree_map :: (a->b) -> Mtree a -> Mtree b
mtree_map f (Mnode a mts)
  = Mnode (f a) (map (mtree_map f) mts)


--14-2 Tree: 

data Ttree3 t = Nil1 | Node4 t (Ttree3 t) (Ttree3 t) (Ttree3 t)


avg :: Ttree3 Double -> Double
avg Nil1 = 0.0
avg tt = let (sum,count) = avgNum tt
         in sum / count

avgNum :: Ttree3 Double -> (Double,Double)
avgNum Nil1 = (0.0,0.0)
avgNum (Node4 n left mid right) = 
    let (suml,countl) = avgNum left
        (summ,countm) = avgNum mid
        (sumr,countr) = avgNum right
    in ((suml + summ + sumr + n),(countl + countm + countr + 1))






 







