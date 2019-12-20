--  Subject  : UniMelb 2019 SM1 COMP90048 Declarative Programming
--  File     : Proj1FullTest.hs
--  Author   : Andy Chuang
--  Origin   : Mon Apr 9 2019 
--  Purpose  : Test program for proj1 performance evaluation. This program will 
--             auto-generate all possible targets as testcases and report average 
--             guesses count. You can use this program to assess your next-guess
--             srategy.
--  Usage    : Compile - $ ghc -O2 --make Proj1FullTest 
--           : Run ./Proj1FullTest


module Main where

import Proj1
import Data.List
import Data.Maybe

main :: IO ()
main = do

    let allPitches = ["A1", "B1", "C1", "D1", "E1", "F1", "G1",
                      "A2", "B2", "C2", "D2", "E2", "F2", "G2",
                      "A3", "B3", "C3", "D3", "E3", "F3", "G3"]

    let allTests = (genCombinations 3 allPitches)

    let guessResults = map runTest allTests

    -- Report overall Performance:
    let avgGuess = (/) (fromIntegral(sum guessResults)) (fromIntegral(length guessResults))
    putStrLn $ "Avg :" ++ show avgGuess ++ " guesses."
    putStrLn $ "Total : " ++ show (length guessResults) ++ " Test."
    putStrLn $ "Total : " ++ show (sum guessResults) ++ " guesses."
    putStrLn $ "Max : " ++ show (maximum guessResults) ++ " guesses."
    putStrLn $ "Min : " ++ show (minimum guessResults) ++ " guesses."



runTest :: [String] -> Int
runTest target = loop (map fromJust (map toPitch target)) 
                      (fst(initialGuess)) 
                      (snd(initialGuess)) 
                      1

loop :: [Pitch] -> [Pitch] -> Proj1.GameState -> Int -> Int
loop target guess other guesses = if (feedback target guess) == (3,0,0)
                                  then guesses
                                  else
                                      loop target
                                           (fst(nextGuess (guess,other) (feedback target guess))) 
                                           (snd (nextGuess (guess,other) (feedback target guess))) 
                                           (guesses+1)

genCombinations :: Int -> [a] -> [[a]]
genCombinations 0 _  = [ [] ]
genCombinations n xs = [ y:ys | y:xs' <- tails xs
                           , ys <- genCombinations (n-1) xs']

