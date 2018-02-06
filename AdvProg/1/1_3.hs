import Data.Char

main :: IO ()
main = do s <- getContents
          putStr(unlines(take 10 (atmark (lines s))))

atmark [] = []
atmark(x:xs) = if (elem '@' x) == True then x:(atmark xs) else atmark xs
