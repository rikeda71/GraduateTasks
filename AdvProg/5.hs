# https://qiita.com/dkatsura/items/20a86d863ea4ed088e63参考
# infFizzBuzz [1..]の様に使う

fizzbuzz n | n `mod` 15 == 0 = "FizzBuzz"
           | n `mod` 5  == 0 = "Fizz"
           | n `mod` 3  == 0 = "Buzz"
           | otherwise = show n

infFizzBuzz [] = []
infFizzBuzz (x:xs) = [fizzbuzz x] ++ infFizzBuzz xs
