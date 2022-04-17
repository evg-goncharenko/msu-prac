-- Renderer - module that maps a game state into a gloss picture

module Renderer(render) where

import State
import Playfield
import Constants
import Graphics.Gloss


-- Playfield dimensions

padding :: Int
padding = (768 - (20 * cellSize)) `quot` 2


wellWidth :: Int
wellWidth = 10 * cellSize
wellHeight :: Int
wellHeight = 20 * cellSize

wallWidth = wellWidth + 2 * padding
wallHeight = wellHeight + 2 * padding

wellColor = black
wallColor = dark (dark white)

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
renderWell well =
  pictures (map cellToPicture (coordCells well))
    where
      cellToPicture (px,py,c)
        | py > (-3)  = pictures []
        | c == Empty = pictures []
        | otherwise  = renderCell (px, py) (cellColor c)

fillBorder :: (Float, Float) -> (Float, Float) -> Picture
fillBorder (dx, dy) (width, height) = color borderColor $
                             translate x y $ 
                              rectangleSolid width height
                              where x = (dx * windowScale - fromIntegral windowWidth / 2)
                                    y = (dy * windowScale - fromIntegral windowHeight / 2)

drawBorders :: Picture
drawBorders  = pictures 
              [ fillBorder (fromIntegral columns / 2, 0) size1
              , fillBorder (fromIntegral columns / 2, fromIntegral rows) size1
              , fillBorder (0, fromIntegral rows / 2) size2
              , fillBorder (fromIntegral columns, fromIntegral rows / 2) size2 
              ]
              where size1 = (fromIntegral windowWidth + windowScale, windowScale)
                    size2 = (windowScale, fromIntegral windowHeight)

-- Game State renderer
render :: State -> Picture
render gameState =
  case screen gameState of
    NameField -> drawNameField gameState
    Table -> renderTable gameState
    Menu -> renderMenu gameState
    Game -> renderGame gameState

drawNameField :: State -> Picture
drawNameField state = pictures 
    [ drawBorders
    , drawTextField state
    ]

drawTextField :: State -> Picture
drawTextField state = pictures
        [ color foodColor $ 
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
       ,  color foodColor $ 
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
      [ drawBorders
      , drawTableElements gameState
      ]

renderMenu :: State -> Picture
renderMenu gameState = menu
  where
    menu = pictures
      [ drawBorders
      , drawMenuElements gameState
      ]

drawTableElements :: State -> Picture
drawTableElements state = pictures(
          ([
            color foodColor $ 
            translate (- (fromIntegral (length ("Top 10 players")) * 14)) 250 $
            scale 0.4 0.4 $
            text "Top 10 players"
          , color foodColor $ 
            translate (- (fromIntegral (length ("Exit")) * 9)) (-280) $
            scale 0.3 0.3 $
            text "Exit"
          ]) ++ (tableTiles (records state) 50))

tableTiles :: Records -> Float -> [Picture]
tableTiles [] _ = []
tableTiles (x:xs) offset = 
        ((color blue $ 
        translate (-100) (250 - offset) $
        scale 0.2 0.2 $
        text ((fst x) ++ " " ++ show (snd x))) : (tableTiles xs (offset + 50.0)))

drawMenuElements :: State -> Picture
drawMenuElements state = pictures
      [ color borderColor $ 
        translate (- (fromIntegral (length ("Tetris")) * 11)) 200 $
        scale 0.4 0.4 $
        text "Tetris"
      , color (if ((selected state) == 0) then foodColor else blue) $ 
        translate (- (fromIntegral (length ("Play")) * 9)) (50) $ 
        scale 0.3 0.3 $ 
        text "Play"
      , color (if ((selected state) == 1) then foodColor else blue) $ 
        translate (- (fromIntegral (length ("Top 10 players")) * 10)) (-50) $ 
        scale 0.3 0.3 $ 
        text "Top 10 players"
      , color (if ((selected state) == 2) then foodColor else blue) $ 
        translate (- (fromIntegral (length ("Change your name")) * 11)) (-150) $ 
        scale 0.3 0.3 $ 
        text "Change your name"
      , color foodColor $ 
        translate (- (fromIntegral (length (name state)) * 8)) (-280) $ 
        scale 0.2 0.2 $ 
        text (name state)
        ]

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
