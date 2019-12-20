data Ttree3 t = Nil1 | Node4 t (Ttree3 t) (Ttree3 t) (Ttree3 t)


avg :: Ttree3 Double -> Double
avg tt = let (sum,count) = avgNum tt
         in sum / count

avgNum :: Ttree3 Double -> (Double,Double)
avgNum Nil1 = (0.0,0.0)
avgNum (Node4 n left mid right) =
  let (suml,countl) = avgNum left
      (summ,countm) = avgNum mid
      (sumr,countr) = avgNum right
  in ((n + suml + summ + sumr),(1+countl + countm + countr)) 