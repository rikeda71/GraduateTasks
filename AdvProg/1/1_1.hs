import Data.Char

main :: IO ()
main = do s <- getContents
          replace(lines s)

replace [] = return ()
replace (x:xs) = do
                 putStrLn (map toLower x)
                 putStrLn (map toUpper x)
                 replace(xs)
