-- Logic module
-- Functions for updating game state and responding to user input

module Logic(updateGameState, handleEvent) where

import State
import Piece ( Piece, pieceCW, pieceCCW, validPos, randomPiece )
import Playfield
    ( Well, renderPiece, pieceCollides, clearAndCountFilledRows )
import Constants ()
import Graphics.Gloss ()
import Graphics.Gloss.Interface.Pure.Game
import System.Random ( Random(randomR), StdGen )

-- Piece falling velocity, in cells/second
pieceVelocity :: Float
pieceVelocity = 10

acceleratedPieceVelocity :: Float
acceleratedPieceVelocity = 30

effectivePieceVelocity :: State -> Float
effectivePieceVelocity s | accelerate s = acceleratedPieceVelocity | otherwise = pieceVelocity

-- Time to wait before dropping piece again
effectivePiecePeriod :: State -> Float
effectivePiecePeriod s = 1.0 / effectivePieceVelocity s

handleEvent :: Event -> State -> State
handleEvent event state = 
  case screen state of
    NameField -> handleText event state
    Menu -> handleMenu event state
    Table -> handleTable event state
    Game -> handleGame event state

handleText :: Event -> State -> State
handleText (EventKey (SpecialKey keyButton) Down _ _) state = 
  case keyButton of
    KeyEnter -> if ((length(name state)) /= 0) then state {screen = Menu} else state
    _ -> state
handleText (EventKey (Char key) Down _ _) state = 
    case key of
        '\b' -> state {name = deleteLast (name state) }
        letter -> if (length (name state) < 28) then 
                    state {name = (name state) ++ [letter]} 
                  else 
                    state
    
handleText _ state = state

deleteLast :: [a] -> [a]
deleteLast []     = []
deleteLast [_]    = []
deleteLast (h : t)  =[h] ++ deleteLast t

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
rotateCW = transformPiece pieceCW -- I feel SO badass for doing this!
rotateCCW = transformPiece pieceCCW

-- Update function passed to gloss
updateGameState :: Float -> State -> State
updateGameState t s =   
  case screen s of
    NameField -> s
    Menu -> s
    Table -> s
    Game -> unityStyleUpdate (s {time = time s + t, deltaTime = t})

-- my update function
unityStyleUpdate :: State -> State
unityStyleUpdate s
  | secondsToNextMove stateWithUpdatedClocks <= 0 = applyMove stateWithUpdatedClocks {secondsToNextMove = effectivePiecePeriod s}
  | otherwise                                     = stateWithUpdatedClocks
    where
      stateWithUpdatedClocks = s {secondsToNextMove = secondsToNextMove s - deltaTime s}

-- Refactored from applyMove. We also needed it to move left-right and rotate a piece
canPieceBeAt :: Piece -> (Int, Int) -> Well -> Bool
canPieceBeAt piece coord well = insidePlayfield && not colliding
  where
    insidePlayfield = validPos coord piece
    colliding = pieceCollides piece coord well

-- Moves the current piece one cell down
applyMove :: State -> State
applyMove s
  | nextPosInvalid    = handleFullRows (fixPiece s)
  | otherwise         = s {piecePos = piecePos'}
    where
      nextPosInvalid = not (canPieceBeAt (piece s) piecePos' (well s))
      piecePos' = (fst (piecePos s), snd (piecePos s) - 2)

-- Fixes the falling piece to its current position and resets the piece to a new one
fixPiece :: State -> State
fixPiece s
  | snd (piecePos s) > (-2) = resetGameState s -- reset game state when 'fixing' a piece that overflows the well
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
handleFullRows :: State -> State
handleFullRows s = s {well = fst result, score = score s + linesToScore (snd result)}
  where result = clearAndCountFilledRows (well s)

-- The scoring system
linesToScore :: Int -> Int
linesToScore 0 = 0
linesToScore 1 = 40
linesToScore 2 = 100
linesToScore 3 = 300
linesToScore 4 = 1200
linesToScore _ = error "Invalid cleared Line count"
