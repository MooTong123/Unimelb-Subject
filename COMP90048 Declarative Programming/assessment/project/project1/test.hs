
-- Author:  Moo Tong
-- Date:  28/03/2019
-- Email: mtm@student.unimelb.edu.au
-- ID;    1004452


module Proj1 (Pitch, toPitch, feedback,
GameState, initialGuess,nextGuess) where

import Data.List

type GameState = [[Pitch]] 

data Note = A | B | C | D | E | F | G
  deriving (Eq) 

data Octave = One | Two | Three
 deriving (Eq)

data Pitch = Pitch Note Octave
 deriving (Eq)

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



--function: to judge if the string is Pitch type
ifPitch :: String -> Bool
ifPitch s = if (length s == 2 && elem (tail s) ["1","2","3"] 
        && elem (head s) ['A','B','C','D','E','F','G'])  then True
              else False

--to get the note from the string
toNote :: String -> Note
toNote s   | noteStr == 'A' = A
           | noteStr == 'B' = B
           | noteStr == 'C' = C
           | noteStr == 'D' = D
           | noteStr == 'E' = E
           | noteStr == 'F' = F
           | noteStr == 'G' = G
 where noteStr = head (filter ( == (head s)) ['A','B','C','D','E','F','G'])


--to get the octave from the string
toOctave :: String -> Octave
toOctave s | octaveStr == "1" = One
           | octaveStr == "2" = Two
           | octaveStr == "3" = Three
 where octaveStr = head (filter ( == (tail s)) ["1","2","3"])


toPitch :: String -> Maybe Pitch
toPitch s = 
    if (ifPitch s == True) then  
        Just (Pitch (toNote s) (toOctave s))
    else Nothing

noteToStr :: Note -> String
noteToStr n | n == A = "A"
            | n == B = "B"
            | n == C = "C"
            | n == D = "D"
            | n == E = "E"
            | n == F = "F"
            | n == G = "G"

octToStr :: Octave -> String
octToStr o | o == One = "1"
           | o == Two = "2"
           | o == Three = "3"

pitchToStr :: Pitch -> String
pitchToStr (Pitch note octave) = (noteToStr note) ++ (octToStr octave)


--To get the feedback
getOneNote :: Pitch -> Note
getOneNote (Pitch note octave) = note

getNote :: [Pitch] -> [Note]
getNote (p:ps) = getOneNote p : getNote ps
getNote [] = []
 
getOneOctave :: Pitch -> Octave
getOneOctave (Pitch note octave) = octave

getOctave :: [Pitch] -> [Octave]
getOctave (o:os) = getOneOctave o : getOctave os
getOctave [] = []

initialPitchForOneNote :: Note -> [Octave] -> [Pitch]
initialPitchForOneNote n (o:os) = (Pitch n o): initialPitchForOneNote n os
initialPitchForOneNote n [] = []

initialPitch :: [Note] -> [Octave] -> [Pitch]
initialPitch (n:ns) octave = (initialPitchForOneNote n octave) ++ (initialPitch ns octave) 
initialPitch [] octave = []
 

initialGameState :: [Pitch] -> Int -> [[Pitch]]
initialGameState ps 0 = [[]]
initialGameState [] n = []
initialGameState (p:ps) n = map (p:) (initialGameState ps (n-1)) ++ initialGameState ps n


initialGuess :: ([Pitch],GameState)
initialGuess = ([Pitch A One,Pitch B One,Pitch C Two], initialGameState (initialPitch note octave) 3)
  where 
      note   = [A,B,C,D,E,F,G]
      octave = [One,Two,Three]


deletePitch :: Int -> [Pitch] -> GameState -> GameState
deletePitch feedback guess gameState = 
  case feedback of
    0 -> filter (\g -> length (g \\ guess) == 3) gameState
    1 -> filter (\g -> length (g \\ guess) == 2) gameState
    2 -> filter (\g -> length (g \\ guess) == 1) gameState
    3 -> filter (\g -> length (g \\ guess) == 0) gameState 


deleteNote :: Int -> [Pitch] -> GameState -> GameState
deleteNote feedback guess gameState =
  case feedback of
    0 -> filter (\g -> length (getNote g \\ getNote guess) == 3) gameState
    1 -> filter (\g -> length (getNote g \\ getNote guess) == 2) gameState    
    2 -> filter (\g -> length (getNote g \\ getNote guess) == 1) gameState
    3 -> filter (\g -> length (getNote g \\ getNote guess) == 0) gameState

deleteOctave :: Int -> [Pitch] -> GameState -> GameState
deleteOctave feedback guess gameState =
  case feedback of
    0 -> filter (\g -> length (getOctave g \\ getOctave guess) == 3) gameState
    1 -> filter (\g -> length (getOctave g \\ getOctave guess) == 2) gameState
    2 -> filter (\g -> length (getOctave g \\ getOctave guess) == 1) gameState
    3 -> filter (\g -> length (getOctave g \\ getOctave guess) == 0) gameState
        

feedback :: [Pitch] -> [Pitch] -> (Int,Int,Int)
feedback target guess = (correctPitch,correctNote,correctOctave)
  where correctPitch  = 3 - length(target \\ guess)
        correctNote   = 3 - length(getNote target \\ getNote guess) - correctPitch
        correctOctave = 3 - length(getOctave target \\ getOctave guess) - correctPitch

remainingFeedback :: GameState -> [[(Int,Int,Int)]]
remainingFeedback (g:gs) = [[feedback g remainingGameState | remainingGameState <- gs ]]

socresforOne :: (Int,Int,Int) -> Int
socresForOne (correctPitch,correctNote,correctOctave) = 3 * correctPitch + 2 * correctNote + correctOctave
socresforOne _ = 0


scoresForList :: [(Int,Int,Int)] -> Int
scoresForList (l:ls) = socresforOne l + scoresForList ls
scoresForList [] = 0
                                                  

socresOfRemainingFeedback :: [[(Int,Int,Int)]] -> [Int]
socresOfRemainingFeedback (s:ss) = scoresForList s : socresOfRemainingFeedback ss
socresOfRemainingFeedback [] = [] 





{-
findBestGuess :: [(Int,Int,Int)] -> GameState -> [Maybe Pitch]
findBestGuess remainingFeedback remainingGameState  
  | ((elemIndex maxNum listOfScores) /= Nothing) = Just remainingGameState!!(elemIndex maxNum listOfScores)
  | otherwise = Nothing
where 
  maxNum = (maximum listOfScores)
  listOfScores = (socresOfRemainingFeedback remainingFeedback)

-}


findBestGuess :: [[(Int,Int,Int)]] -> GameState -> [Pitch]
findBestGuess remainingFeedback remainingGameState  
  = remainingGameState!!((elemIndices maxNum listOfScores)!!0)
  where 
    maxNum = (maximum listOfScores)
    listOfScores = (socresOfRemainingFeedback remainingFeedback)


nextGuess :: ([Pitch],GameState) -> (Int,Int,Int) -> ([Pitch],GameState)
nextGuess (guess, preGameState) (correctPitch,correctNote,correctOctave) =
  ((findBestGuess remainFeedback remainingGameState),remainingGameState)
  where  remainFeedback = remainingFeedback preGameState
         deleteP = deletePitch correctPitch guess targets
         deletePN = deleteNote (correctNote + correctPitch) guess deleteP
         remainingGameState = deleteOctave (correctOctave + correctPitch) guess deletePN
         targets = preGameState \\ [guess]


