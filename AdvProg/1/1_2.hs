import Data.Char
import Text.Regex

main :: IO ()
main = do s <- getLine
          print(count s)

count "" = 0
count (x:xs) = (length (matchRegexAll (mkRegex "([0-9])") [x])) + (count xs)
