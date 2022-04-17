-- Game State

module State where

import Playfield
import Piece
import System.Random

type Record = (String, Int)

type Records = [Record]

data Screen = NameField | Menu | Table | Game deriving Eq

data State = State
    { well :: Well
    , time :: Float
    , deltaTime :: Float
    , secondsToNextMove :: Float
    , piece :: Piece
    , piecePos :: (Int, Int)
    , randomSeed :: StdGen
    , score :: Int
    , accelerate :: Bool
    , screen :: Screen
    , name :: String
    , selected :: Int
    , records :: Records
    }

initialGameState :: State
initialGameState = State
    { well = emptyWell
    , time = 0
    , deltaTime = 0
    , secondsToNextMove = 0
    , piece = tetrominoO
    , piecePos = (0, 0)
    , randomSeed = mkStdGen 0 -- found better way!
    , score = 0
    , accelerate = False
    , screen = Menu
    , name = "Player"
    , selected = 0
    , records = [("Eugene", 1200), 
             ("Anastasia", 1200), 
             ("player1", 300), 
             ("player2", 100), 
             ("player3", 40),
             ("player4", 40)]
    }

-- Resets a game state, maintaining the random seed
resetGameState :: State -> State
resetGameState s = initialGameState {randomSeed = randomSeed s}
