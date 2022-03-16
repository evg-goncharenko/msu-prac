-- Logic module

module Logic where

import State
import Piece
import Playfield

-- Piece falling velocity, in cells/second
pieceVelocity :: Float
-- TODO

-- Time to wait before dropping piece again
waitPeriod :: State -> Float
-- TODO

-- Transforms the falling piece, if possible
transformPiece :: (Piece -> Piece) -> State -> State
-- TODO

-- Rotates the falling piece clockwise, if possible
rotateCW :: State -> State
-- TODO

-- Moves the current piece one cell down
applyMove :: State -> State
-- TODO

-- Fixes the falling piece to its current position and resets the piece to a new one
fixPiece :: State -> State
-- TODO

-- Removes filled rows and changes the score accordingly
handleFullRows :: State -> State
-- TODO

-- The scoring system
linesToScore :: Int -> Int
linesToScore 0 = 0
linesToScore 1 = 50
linesToScore 2 = 100
linesToScore 3 = 400
linesToScore 4 = 800
linesToScore _ = error "Invalid cleared Line count"
