-- Game State

module State where

import Playfield
import Piece
import System.Random
import DBase()

import Database.HDBC.Sqlite3
import Database.HDBC()

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
    , connection :: Connection
    , playerId :: Int
    }

initialGameState :: Connection -> Records -> Int -> State
initialGameState new_conn new_record new_id = State
    { well = emptyWell
    , time = 0
    , deltaTime = 0
    , secondsToNextMove = 0
    , piece = tetrominoO
    , piecePos = (0, 0)
    , randomSeed = mkStdGen 0
    , score = 0
    , accelerate = False
    , screen = Menu
    , name = "Player"
    , selected = 0
    , records = new_record
    , connection = new_conn
    , playerId = new_id
    }

-- Resets a game state, maintaining the random seed
resetGameState :: State -> Connection -> Records -> Int -> State
resetGameState s c r i = (initialGameState c r i) {randomSeed = (randomSeed s), name = (name s)}
