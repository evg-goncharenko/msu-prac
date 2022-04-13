module Main(main) where

import Graphics.Gloss

import State
import Renderer
import Logic
import System.Random
import Constants
    
main :: IO ()
main = play window backgroundColor fps initialGameState render handleEvent updateGameState
