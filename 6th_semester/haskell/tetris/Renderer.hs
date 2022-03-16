-- Renderer - module that maps a game state

module Renderer where

import State
import Playfield

-- Let's start with rendering an empty well.

-- Playfield dimensions
cellSize = 35
padding = (768 - (20 * cellSize)) `quot` 2

wellWidth = 10 * cellSize
wellHeight = 20 * cellSize

wallWidth = wellWidth + 2 * padding
wallHeight = wellHeight + 2 * padding

-- Colors
wellColor = black

-- Function that renders a single cell
renderCell :: (Int, Int) -> Color -> Picture
-- TODO

-- Game State renderer
render :: State -> Picture
-- TODO
    