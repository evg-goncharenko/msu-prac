module Constants where

import Graphics.Gloss 
import System.Random 

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

foodColor :: Color
foodColor = makeColorI 242 43 41 255

borderColor :: Color 
borderColor = makeColorI 119 194 46 255
