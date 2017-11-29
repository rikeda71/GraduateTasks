--2016
--(1)
foo2016 :: [Integer] -> Bool
foo2016 (x:xs) | mod x 5 /= 0 = False
           | otherwise = foo2016 xs
foo2016 [] = True

--(2)
bar2016 :: Integer -> [(Integer, Integer)]
bar2016 n = [(i, j) | i <- [0..n], j <- [0..i^2], mod (i + j) 2 /= 0]

--2015
--(1)
foo2015 :: [Integer] -> Integer
foo2015 (x:xs) | mod x 2 /= 0 = x + foo2015 xs
               | otherwise = foo2015 xs
foo2015 [] = 0

--(2)
bar2015 :: Integer -> [(Integer, Integer)]
bar2015 n = [(i, j) | i <- [2..n], j <- [i..n], i * j <= n]

--2014
--(1)
foo2014 :: [Integer] -> Bool
foo2014 (x:xs) = x < 0 || foo2014 xs
foo2014 [] = False

--(2)
bar2014 :: Integer -> [(Integer, Integer)]
bar2014 n = [(i, j) | i <- [1..n], j <- [i..n], mod (i * j) 8 == 0]
