-- Constants module
-- Initializing sizes, colors and resolutions

module Constants where

import Graphics.Gloss 
import System.Random() 

window :: Display
window = InWindow windowName windowSize windowPosition

windowName :: String
windowName = "Tetris"

windowWidth :: Int
windowWidth = 640

windowHeight :: Int
windowHeight = 640

windowSize :: (Int, Int)
windowSize = (windowWidth, windowHeight)

windowPosition :: (Int, Int)
windowPosition = (0, 0)

backgroundColor :: Color
backgroundColor = makeColorI 0 0 0 0

fps :: Int
fps = 60

windowScale :: Float
windowScale = 20

seed :: Int
seed = 19

cellSize :: Int
cellSize = 35

columns :: Int
columns = fst windowSize `div` round windowScale

rows :: Int
rows = snd windowSize `div` round windowScale

-- Piece falling velocity, in cells/second
pieceVelocity :: Float
pieceVelocity = 10

acceleratedPieceVelocity :: Float
acceleratedPieceVelocity = 30

mainColor :: Color
mainColor = makeColorI 255 255 0 255

-- Playfield dimensions
padding :: Int
padding = (768 - (20 * cellSize)) `quot` 2

wellWidth :: Int
wellWidth = 10 * cellSize

wellHeight :: Int
wellHeight = 20 * cellSize

wallWidth :: Int
wallWidth = wellWidth + 2 * padding

wallHeight :: Int
wallHeight = wellHeight + 2 * padding

wellColor :: Color
wellColor = black

wallColor :: Color
wallColor = dark (dark white)
