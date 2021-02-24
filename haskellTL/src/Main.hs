module Main where

import Control.Concurrent ( threadDelay )
import Data.List.Split ( chunksOf )
import Data.Vector ( Vector, (!) )
import qualified Data.Vector as V
import System.Console.Terminal.Size ( size, Window(height, width) )
import System.IO
import System.Random ( getStdGen, Random(randoms) )

type Board  = Vector (Vector Int)
type XCoord = Int
type YCoord = Int

-- | gets the (rows, columns) of a 2D Vector
matrixShape :: Vector (Vector a) -> (YCoord, XCoord)
matrixShape v = (V.length v, V.length $ V.head v)

-- | Returns @1@ if next generation of a cell is alive, else @0@
nextState :: Board -> YCoord -> XCoord -> Int
nextState b y x = fromEnum (cell && 1 < neighbors && neighbors < 4
               || not cell && neighbors == 3)
    where
        cell = toEnum (b ! y ! x) :: Bool
        neighbors = sum [b ! i ! j | i <- [max 0 (y - 1) .. min (rows - 1) (y + 1)]
                                , j <- [max 0 (x - 1) .. min (cols - 1) (x + 1)]]
                    - fromEnum cell
        (rows, cols) = matrixShape b


nextGeneration :: Board -> Board
nextGeneration b = V.map (\i -> V.map (nextState b i)
                 $ V.iterateN cols succ 0)
                 $ V.iterateN rows succ 0
    where
        (rows, cols) = matrixShape b


printBoard :: Board -> IO ()
printBoard b = do
    putStr "\ESC[H"  -- move to top-left of the screen
    V.mapM_ (V.mapM_ (\x -> putChar $ " #" !! x)) b


-- | runs the game forever
runGame :: Board -> IO ()
runGame b = do
    printBoard b
    threadDelay 50000 -- 50000 microseconds == 0.05 seconds delay
    runGame $ nextGeneration b


main :: IO ()
main = do
    Just winsize <- size
    gen <- getStdGen
    let board = V.fromList $ map V.fromList       -- transforming into board
              $ take (height winsize)           -- grabbing @num rows@ chunks
              $ chunksOf (width winsize)        -- chunking into width of terminal
              $ map (\x -> fromEnum $ x <= 0.1) -- randomly populating with 10% density
              (randoms gen :: [Double])         -- random doubles in domain [0, 1)
    runGame board