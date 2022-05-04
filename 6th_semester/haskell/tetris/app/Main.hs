module Main(main) where

import Graphics.Gloss
import Graphics.Gloss.Interface.IO.Game
import State
import Renderer
import Logic
import System.Random
import Constants
import DBase
    
main :: IO ()
main = do
    new_conn <- getConnection
    new_record <- getTop10 new_conn
    new_id <- createNewUser "Player" new_conn
    playIO window backgroundColor fps (initialGameState new_conn new_record new_id) render handleEvent updateGameState
