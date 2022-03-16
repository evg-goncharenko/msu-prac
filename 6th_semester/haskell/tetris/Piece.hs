-- Tetris Piece module

module Piece where

-- The elements of the tuple are x and y coordinates.
data Piece = PieceCoords [(Int, Int)] Color deriving (Show)

-- Piece definitions
-- TODO elem = PieceCoords [(-3, -1), (-1, -1), (1, -1), (3, -1)] (blue)

-- Piece clockwise rotation
pieceCW :: Piece -> Piece
-- TODO

-- Piece counter-clockwise rotation
pieceCCW :: Piece -> Piece
-- TODO
    
-- Checks if a position is valid for a piece with respect to well's bounds
validPos :: (Int, Int) -> Piece -> Bool
-- TODO
