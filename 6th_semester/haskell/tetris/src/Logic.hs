-- Logic module
-- Functions for updating game state and responding to user input

module Logic(updateGameState, handleEvent) where

import State
import Piece ( Piece, pieceCW, pieceCCW, validPos, randomPiece )
import Playfield
    ( Well, renderPiece, pieceCollides, clearAndCountFilledRows )
import Constants
import Graphics.Gloss ()
import Graphics.Gloss.Interface.Pure.Game
import System.Random ( Random(randomR), StdGen )
import DBase

effectivePieceVelocity :: State -> Float
effectivePieceVelocity s | accelerate s = acceleratedPieceVelocity | otherwise = pieceVelocity

-- Time to wait before dropping piece again
effectivePiecePeriod :: State -> Float
effectivePiecePeriod s = 1.0 / effectivePieceVelocity s

-- Initialization of four states: NameField, Menu, Table and Game
handleEvent :: Event -> State -> IO State
handleEvent event state = 
  case screen state of
    NameField -> handleText event state
    Menu -> return (handleMenu event state)
    Table -> return (handleTable event state)
    Game -> return (handleGame event state)

-- Changing username and creating a new one
handleText :: Event -> State -> IO State
handleText (EventKey (SpecialKey keyButton) Down _ _) state = 
  case keyButton of
    KeyEnter -> if ((length(name state)) /= 0) then do
      newId <- createNewUser (name state) (connection state)
      return (state {screen = Menu, playerId = newId}) else return state
    _ -> return state
handleText (EventKey (Char key) Down _ _) state = 
    case key of
        '\b' -> return (state {name = deleteLast (name state) })
        letter -> if (length (name state) < 28) then 
                    return (state {name = (name state) ++ [letter]})
                  else 
                    return state
    
handleText _ state = return state

deleteLast :: [a] -> [a]
deleteLast []     = []
deleteLast [_]    = []
deleteLast (h : t)  =[h] ++ deleteLast t

-- Switching items in the menu
handleMenu :: Event -> State -> State
handleMenu (EventKey (SpecialKey keyButton) Down _ _) state = 
  case keyButton of
      KeyEnter -> case (selected state) of
                    0 -> state {screen = Game}
                    1 -> state {screen = Table}
                    2 -> state {screen = NameField}
                    _ -> state
      KeyDown -> if ((selected state) /= 2) then state {selected = (selected state) + 1} else state
      KeyUp -> if ((selected state) /= 0) then state {selected = (selected state) - 1} else state
      _ -> state
handleMenu _ state = state

-- Switching elements in the results table
handleTable :: Event -> State -> State
handleTable (EventKey (SpecialKey keyButton) Down _ _) state = 
  case keyButton of
    KeyEnter -> state {screen = Menu}
    _ -> state
handleTable _ state = state

handleGame :: Event -> State -> State
handleGame (EventKey (SpecialKey KeyLeft) Down _ _) s = movePiece (-2) s
handleGame (EventKey (SpecialKey KeyRight) Down _ _) s = movePiece 2 s
handleGame (EventKey (SpecialKey KeyDown) Down _ _) s = s {accelerate = True}
handleGame (EventKey (SpecialKey KeyDown) Up _ _) s = s {accelerate = False}
handleGame (EventKey (Char 'a') Down _ _) s = rotateCW s
handleGame (EventKey (Char 's') Down _ _) s = rotateCCW s
handleGame _ s = s

-- Moves the falling piece horizontally, if possible
movePiece :: Int -> State -> State
movePiece offset s
  | canPieceBeAt (piece s) piecePos' (well s) = s {piecePos = piecePos'}
  | otherwise = s
    where
      piecePos' = (fst (piecePos s) + offset, snd (piecePos s))

-- Transforms the falling piece, if possible
transformPiece :: (Piece -> Piece) -> State -> State
transformPiece transform s
  | canPieceBeAt piece' (piecePos s) (well s) = s {piece = piece'}
  | otherwise = s
    where
      piece' = transform (piece s)

-- Rotates the falling piece clockwise, if possible
rotateCW :: State -> State
rotateCW = transformPiece pieceCW

rotateCCW :: State -> State
rotateCCW = transformPiece pieceCCW

-- Update function passed to gloss
updateGameState :: Float -> State -> IO State
updateGameState t s =   
  case screen s of
    NameField -> return (s)
    Menu -> return (s)
    Table -> return (s)
    Game -> unityStyleUpdate (s {time = time s + t, deltaTime = t})

unityStyleUpdate :: State -> IO State
unityStyleUpdate s
  | secondsToNextMove stateWithUpdatedClocks <= 0 = applyMove stateWithUpdatedClocks {secondsToNextMove = effectivePiecePeriod s}
  | otherwise                                     = return (stateWithUpdatedClocks)
    where
      stateWithUpdatedClocks = s {secondsToNextMove = secondsToNextMove s - deltaTime s}

-- Refactored from applyMove. We also needed it to move left-right and rotate a piece
canPieceBeAt :: Piece -> (Int, Int) -> Well -> Bool
canPieceBeAt p coord w = insidePlayfield && not colliding
  where
    insidePlayfield = validPos coord p
    colliding = pieceCollides p coord w

-- Moves the current piece one cell down
applyMove :: State -> IO State
applyMove s
  | nextPosInvalid    = handleFullRows (fixPiece s)
  | otherwise         = return (s {piecePos = piecePos'})
    where
      nextPosInvalid = not (canPieceBeAt (piece s) piecePos' (well s))
      piecePos' = (fst (piecePos s), snd (piecePos s) - 2)

-- Fixes the falling piece to its current position and resets the piece to a new one
fixPiece :: State -> State
fixPiece s
  | snd (piecePos s) > (-2) = resetGameState s (connection s) (records s) (playerId s) -- reset game state when 'fixing' a piece that overflows the well
  | otherwise     = s
    { well = renderPiece (piece s) (piecePos s) (well s)
    , piece = randomPiece (fst reseed)
    , piecePos = (0, 0)
    , randomSeed = snd reseed
    , accelerate = False -- We don't want acceleration to affect next falling piece
    }
      where
        reseed :: (Double, StdGen)
        reseed = randomR (0.0, 1.0) (randomSeed s)

-- Removes filled rows and changes the score accordingly
handleFullRows :: State -> IO State
handleFullRows s = do 
    writeResultToTable (score s + linesToScore (snd result)) (playerId s) (connection s)
    top10 <- getTop10 (connection s)
    return (s {well = fst result, score = score s + linesToScore (snd result), records = top10})
    where result = clearAndCountFilledRows (well s)

-- The scoring system
linesToScore :: Int -> Int
linesToScore 0 = 0
linesToScore 1 = 40
linesToScore 2 = 100
linesToScore 3 = 300
linesToScore 4 = 1200
linesToScore _ = error "Invalid cleared Line count"
