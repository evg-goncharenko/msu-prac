-- Playfield

module Playfield where

import Piece

-- A cell is a rectangle in the playfield - it can either be full or empty
data Cell = Empty | FilledWith Color deriving (Show, Eq)

-- Returns the color of the cell, or black if empty
cellColor :: Cell -> Color
-- TODO

-- Row of cells
data Row = RowOfCells [Cell] deriving (Show)

-- Creates an empty Row
emptyRow = RowOfCells (replicate 10 Empty)

-- Creates an empty Well
emptyWell = WellOfRows (replicate 22 emptyRow)

numberCells :: Row -> [(Int, Cell)]
-- TODO
                                  
-- Renders a piece in the Well
renderPiece :: Piece -> (Int, Int) -> Well -> Well
-- TODO
            
-- Clears a well of its filled rows and returns the filled row count
clearAndCountFilledRows :: Well -> (Well, Int)
-- TODO
