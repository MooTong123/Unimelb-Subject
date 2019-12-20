{- 
 Author:  Moo Tong
 Date:  04/06/2019
 Email:	mtm@student.unimelb.edu.au
 ID;		1004452

 purpose: This file is to implement both the guessing and answering
          parts of a logical guessing game called Musician. For this 
          game,one player is the composer, and the other is the performer.
          The composer begins by selecting a three-pitch musical chord, 
          where each pitch comprises a musical note, one of A,B,C,D,E,F,G,
          and an octave,one of 1,2,3. Once the composer has selected the 
          target chord, the performer repeatedly chooses a similarly 
          defined as a guess and tells it to the composer,who responds 
          by giving the performer the feedback 
          (correct pitches,correct notes,correct octaves). 
          The game will finish once the performer guesses the correct chord.
          The object of the game for the performer is to find the target 
          with the fewest possible guesses.

Types:    There are two important types which are Pitch and GameState.
          Type Pitch consists type Note and Type Octave, which is to 
          represent pitches in this game. Type GameState will hold all 
          remaining possible targets.

Function: There are four important functions,which are toPitch, feedback, 
          initialGuess and nextGuess.
          The toPitch function is to convert type String to type Pitch.
          The feedback function is to return the appropriate feedback 
          by taking target and a guess.
          The initialGuess is to return an initial guess and a game state 
          by taking no input.
          The nextGuess is to return next guess and game state by taking 
          previous guess and feedback.          

Algorthim:The algorithm takes a more probabilistic approach, which means 
          that it calculates the average number of possible targets that 
          will remain for each guess,and choose the pitch with lowest 
          expected number.    
-}          

module Proj1 (Pitch, toPitch, feedback,
GameState, initialGuess,nextGuess) where

import Data.List

--Purpose: to define two important type:Pitch and GameState
type GameState = [[Pitch]] 

data Note = A | B | C | D | E | F | G deriving (Eq) 
data Octave = One | Two | Three deriving (Eq)
data Pitch = Pitch Note Octave deriving (Eq)

instance Show Note where 
  show A = "A"
  show B = "B"
  show C = "C"
  show D = "D"
  show E = "E"
  show F = "F"
  show G = "G"

instance Show Octave where
  show One = "1"
  show Two = "2"
  show Three = "3"

instance Show Pitch where 
  show (Pitch note octave) = show note ++ show octave


--Purpose: toPitch function is to get the pitch from the string
toPitch :: String -> Maybe Pitch
toPitch s = 
    if (ifPitch s == True) then  
        Just (Pitch (toNote s) (toOctave s))
    else Nothing

--Purpose: ifPitch function is to judge if the string is Pitch type.
--         There are two steps of this Algorthim. One is to check
--         if the length of string is 2, and the other is to check 
--         whether the first char is from Note type and the second 
--         is from octave type.        
ifPitch :: String -> Bool
ifPitch s = if (length s == 2 && elem (tail s) ["1","2","3"] 
        && elem (head s) ['A','B','C','D','E','F','G'])  then True
            else False

--Purpose: toNote function is to get the note from the string
toNote :: String -> Note
toNote s   | noteStr == 'A' = A
           | noteStr == 'B' = B
           | noteStr == 'C' = C
           | noteStr == 'D' = D
           | noteStr == 'E' = E
           | noteStr == 'F' = F
           | noteStr == 'G' = G
 where noteStr = head (filter ( == (head s)) ['A','B','C','D','E','F','G'])


--Purpose: toOctave function is to get the octave from the string
toOctave :: String -> Octave
toOctave s | octaveStr == "1" = One
           | octaveStr == "2" = Two
           | octaveStr == "3" = Three
 where octaveStr = head (filter ( == (tail s)) ["1","2","3"])

--Purpose: pitchToStr function is to convert pitch to string
pitchToStr :: Pitch -> String
pitchToStr (Pitch note octave) = (noteToStr note) ++ (octToStr octave)

--Purpose: noteToStr function is to convert note to string
noteToStr :: Note -> String
noteToStr n | n == A = "A"
            | n == B = "B"
            | n == C = "C"
            | n == D = "D"
            | n == E = "E"
            | n == F = "F"
            | n == G = "G"

--Purpose: octToStr function is to convert octave to string
octToStr :: Octave -> String
octToStr o | o == One = "1"
           | o == Two = "2"
           | o == Three = "3"

--Purpose: getNote function is to get a list of note from a list of pitch
getNote :: [Pitch] -> [Note]
getNote (p:ps) = getOneNote p : getNote ps
getNote [] = []

--Purpose: getOneNote function is to get a note from a pitch
getOneNote :: Pitch -> Note
getOneNote (Pitch note octave) = note

--Purpose: getOctave function is to get a list of octave from a list of pitch
getOctave :: [Pitch] -> [Octave]
getOctave (o:os) = getOneOctave o : getOctave os
getOctave [] = []

--Purpose: getOneOctave function is to get a octave from a pitch
getOneOctave :: Pitch -> Octave
getOneOctave (Pitch note octave) = octave

--Purpose: initialGuess function is to return a initial guess and the initial 
--         game state. In order to get beter efficiency, hardcode the initial 
--          guess as (C2,D3,E2).
initialGuess :: ([Pitch],GameState)
initialGuess = ([Pitch C Two,Pitch D Three,Pitch E Two],initGameState)
  where note   = [A,B,C,D,E,F,G]
        octave = [One,Two,Three]
        initGameState = initialGameState (initialPitch note octave) 3
        

--Purpose: initialGameState function is to return the initial game state by
--         using combination, because the order of pitches in target
--         is irrelevant.
initialGameState :: [Pitch] -> Int -> [[Pitch]]
initialGameState ps 0 = [[]]
initialGameState [] n = []
initialGameState (p:ps) n = map (p:) (initialGameState ps (n-1)) 
                            ++ initialGameState ps n

-- Purpose: initialPitch function is to get all the Pitch like A1,A2...G3
initialPitch :: [Note] -> [Octave] -> [Pitch]
initialPitch (n:ns) octave = (initialPitchForOneNote n octave) 
                             ++ (initialPitch ns octave) 
initialPitch [] octave = []

--Purpose: initialPitchForOneNote function is to get all Pitch from a note
--         and a list of Octave.
initialPitchForOneNote :: Note -> [Octave] -> [Pitch]
initialPitchForOneNote n (o:os) = (Pitch n o): initialPitchForOneNote n os
initialPitchForOneNote n [] = []

--Purpose: nextGuess function is to return next guess and remaining game 
--         state. The next guess is calculated by feedback, and the 
--         remaining game state is obtained by deleting impossible targets
--         containing impossible Pitch,note and Octave. 
--         \\ is the list difference by importing Data.List.
nextGuess :: ([Pitch],GameState) -> (Int,Int,Int) -> ([Pitch],GameState)
nextGuess (guess, preGameState) (correctPitch,correctNote,correctOctave) =
  ((findBestGuess remainFeedback remainGameState),remainGameState)
 where  remainFeedback = remainingFeedback remainGameState remainGameState
        deleteP = deletePitch correctPitch guess targets
        deletePN = deleteNote (correctNote + correctPitch) guess deleteP
        remainGameState 
            = deleteOctave (correctOctave + correctPitch) guess deletePN
        targets = preGameState \\ [guess]

--Purpose: deletePitch function is to delete the targets containing 
--         impossible Pitch from the game state, and return a new 
--         game state by using filter function.
deletePitch :: Int -> [Pitch] -> GameState -> GameState
deletePitch feedback guess gameState = 
  case feedback of
    0 -> filter (\g -> length (g \\ guess) == 3) gameState
    1 -> filter (\g -> length (g \\ guess) == 2) gameState
    2 -> filter (\g -> length (g \\ guess) == 1) gameState
    3 -> filter (\g -> length (g \\ guess) == 0) gameState 

--Purpose: deleteNote function is to delete the targets containing impossible
--         Note from the game state, and return a new game state by using 
--         filter function.
deleteNote :: Int -> [Pitch] -> GameState -> GameState
deleteNote feedback guess gameState =
  case feedback of
    0 -> filter (\g -> length (getNote g \\ getNote guess) == 3) gameState
    1 -> filter (\g -> length (getNote g \\ getNote guess) == 2) gameState    
    2 -> filter (\g -> length (getNote g \\ getNote guess) == 1) gameState
    3 -> filter (\g -> length (getNote g \\ getNote guess) == 0) gameState

--Purpose: deleteOctave function is to delete the targets containing 
--         impossible Octave from the game state, and return a new game 
--         state by using filter function.
deleteOctave :: Int -> [Pitch] -> GameState -> GameState
deleteOctave feedback guess gameState =
  case feedback of
    0 -> filter (\g -> length (getOctave g \\ getOctave guess) == 3) gameState
    1 -> filter (\g -> length (getOctave g \\ getOctave guess) == 2) gameState
    2 -> filter (\g -> length (getOctave g \\ getOctave guess) == 1) gameState
    3 -> filter (\g -> length (getOctave g \\ getOctave guess) == 0) gameState

--Purpose: to find the best guess by searching for the lowest expected number
--         of each pitch
findBestGuess :: [[(Int,Int,Int)]] -> GameState -> [Pitch]
findBestGuess remainingFeedback remainingGameState  
  = remainingGameState!!((elemIndices minNum listOfScores)!!0)
  where minNum = (minimum listOfScores)
        listOfScores = (socresOfRemainFeedback remainingFeedback)

--Purpose: to get the remaining feedback for each possible targets by 
--         comparing every two targets in remaining game state
remainingFeedback :: GameState ->GameState -> [[(Int,Int,Int)]]
remainingFeedback (g:gs) same 
  = [feedback g remainGameState | remainGameState <- (filter (/=g) same)] 
    : remainingFeedback gs same
remainingFeedback [] _ = []

--Purpose: to get the feedback by comparing two Pitches.
feedback :: [Pitch] -> [Pitch] -> (Int,Int,Int)
feedback target guess = (pitch,note,octave)
  where pitch  = 3 - length(target \\ guess)
        note   = 3 - length(getNote target \\ getNote guess) - pitch
        octave = 3 - length(getOctave target \\ getOctave guess) - pitch

--Purpose: to get a list of scores(Expected Number) from the remaining
--         each targets 
socresOfRemainFeedback :: [[(Int,Int,Int)]] -> [Double]
socresOfRemainFeedback (s:ss) = scoresForList s : socresOfRemainFeedback ss
socresOfRemainFeedback [] = [] 

--Purpose: to get the score for one pitch comparing with other pitches 
--         in game state countTimes is to sort the list first and then 
--         count the number of the same feedback to form a list of Int
scoresForList :: [(Int,Int,Int)] -> Double
scoresForList [] = 0
scoresForList l = expectedNumber countTimes
  where countTimes = [length l | l <- group (sort l)]

--Purpose: to get the expected number from a list of Int(the number 
--         of the same feedback)
expectedNumber :: [Int] -> Double
expectedNumber [] = 0
expectedNumber list = 
  sum [fromIntegral a * fromIntegral a | a <- list] / fromIntegral (sum list)
