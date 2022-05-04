-- Renderer - module that maps a game state into a gloss picture

module Renderer(render) where

import State
import Playfield
import Constants
import Graphics.Gloss
import DBase()

-- Convert from playfield coordinate to screen coordinate
playfieldToScreen :: (Int, Int) -> (Int, Int)
playfieldToScreen (px, py) = (sx, sy) where
  sx = (px * cellSize) `quot` 2
  sy = (11 * cellSize) + (py * cellSize) `quot` 2

-- Function that renders a single cell
renderCell :: (Int, Int) -> Color -> Picture
renderCell (px, py) col = translate (fromIntegral sx) (fromIntegral sy) (color col (rectangleSolid sz sz))
  where
    sx = fst transformed
    sy = snd transformed
    sz = 0.9 * fromIntegral cellSize
    transformed = playfieldToScreen (px, py)

-- Renders Well playfield to Picture
renderWell :: Well -> Picture
renderWell w =
  pictures (map cellToPicture (coordCells w))
    where
      cellToPicture (px,py,c)
        | py > (-3)  = pictures []
        | c == Empty = pictures []
        | otherwise  = renderCell (px, py) (cellColor c)

-- Game State renderer
render :: State -> IO Picture
render gameState =
  return (case screen gameState of
    NameField -> drawNameField gameState
    Table -> renderTable gameState
    Menu -> renderMenu gameState
    Game -> renderGame gameState)

drawNameField :: State -> Picture
drawNameField state = pictures 
    [ drawTextField state ]

-- Filling the user name
drawTextField :: State -> Picture
drawTextField state = pictures
        [ color mainColor $ 
          translate (-200) 0 $
          scale 0.35 0.35 $
          text "Enter your name"
       ,  color blue $ 
          translate (-250) (-100) $ 
          scale 0.3 0.3 $ 
          text ((name state) ++ "|")
       ,  color blue $ 
          translate (-250) (-100) $ 
          scale 0.3 0.3 $ 
          text (name state)
       ,  color mainColor $ 
          translate (-250) (-200) $ 
          scale 0.3 0.3 $ 
          text (
            if (length (name state) /= 0) then 
              "Press ENTER to continue" 
            else 
              ""
          )
        ]

renderTable :: State -> Picture
renderTable gameState = table
  where
    table = pictures
      [ drawTableElements gameState ]

renderMenu :: State -> Picture
renderMenu gameState = menu
  where
    menu = pictures
      [ drawMenuElements gameState ]

-- Filling elements of the results table
drawTableElements :: State -> Picture
drawTableElements state = pictures(
          ([
            color mainColor $ 
            translate (- (fromIntegral (length ("Top 10 players")) * 14)) 250 $
            scale 0.4 0.4 $
            text "Top 10 players"
          , color mainColor $ 
            translate (- (fromIntegral (length ("Exit")) * 9)) (-280) $
            scale 0.3 0.3 $
            text "Exit"
          ]) ++ (tableTiles (records state) 50))

-- Drawing the results table
tableTiles :: Records -> Float -> [Picture]
tableTiles [] _ = []
tableTiles (x:xs) offset = 
        ((color blue $ 
        translate (-100) (250 - offset) $
        scale 0.2 0.2 $
        text ((fst x) ++ " " ++ show (snd x))) : (tableTiles xs (offset + 50.0)))

-- Drawing the menu
drawMenuElements :: State -> Picture
drawMenuElements state = pictures
      [ color yellow $ 
        translate (- (fromIntegral (length ("Tetris")) * 11)) 200 $
        scale 0.4 0.4 $
        text "Tetris"
      , color (if ((selected state) == 0) then mainColor else blue) $ 
        translate (- (fromIntegral (length ("Play")) * 9)) (50) $ 
        scale 0.3 0.3 $ 
        text "Play"
      , color (if ((selected state) == 1) then mainColor else blue) $ 
        translate (- (fromIntegral (length ("Top 10 players")) * 10)) (-50) $ 
        scale 0.3 0.3 $ 
        text "Top 10 players"
      , color (if ((selected state) == 2) then mainColor else blue) $ 
        translate (- (fromIntegral (length ("Change your name")) * 11)) (-150) $ 
        scale 0.3 0.3 $ 
        text "Change your name"
      , color mainColor $ 
        translate (- (fromIntegral (length (name state)) * 8)) (-280) $ 
        scale 0.2 0.2 $ 
        text (name state)
        ]

-- Render entire game with states
renderGame :: State -> Picture
renderGame gameState = pictures [ walls, playfield, activePiece, guiStuff ]
  where
    walls = color wallColor (rectangleSolid (fromIntegral wallWidth) (fromIntegral wallHeight))
    playfield = pictures
      [ color wellColor (rectangleSolid (fromIntegral wellWidth) (fromIntegral wellHeight))
      , renderWell (well gameState)
      ]
    activePiece = renderWell (renderPiece (piece gameState) (piecePos gameState) emptyWell)
    guiStuff = translate (-600.0) 200.0 (scale 0.4 0.4 (pictures [playerScore]))
      where
        playerScore = color white (Text scoreText)
        scoreText = "SCORE: " ++ show (score gameState)
