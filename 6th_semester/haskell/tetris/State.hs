-- Game State

module State where

import Playfield
import Piece

data State = State
    { well :: Well
    , time :: Float
    , piece :: Piece
    , randomSeed :: StdGen
    , score :: Int
    }
    
-- Resets a game state, maintaining the random seed
resetGameState :: State -> State
-- TODO