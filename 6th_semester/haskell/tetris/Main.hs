module Main(main) where

import State
import Renderer
import Logic

window :: Display
window = InWindow "Tetris" (1900, 1200) (400, 400)

background :: Color
background = black
fps = 60
    
main :: IO ()
main = do
-- TODO
