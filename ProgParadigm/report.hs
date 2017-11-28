import Data.List
-- 1.問2.4.10
deleteOne x [] = []
deleteOne x (y:ys) = if x == y then ys else [y] ++ deleteOne x ys
deleteAll x xs = filter(/= x) xs


-- 2.問2.5.1
-- レポートに書く


-- 3.問2.10.1
myDrop :: Integer -> [a] -> [a]
myDrop x (y:ys) = if x == 0 then (y:ys) else myDrop (x - 1) ys
myDrop x [] = []

-- 4.問2.11.3
chokkaku :: Integer -> [(Integer, Integer, Integer)]
chokkaku n = [(x, y, z) | x <- [1..n], y <- [1..n], z <- [1..n], x < y, y < z, z <= n, x^2 + y^2 == z^2]


-- 5.問2.12.5
-- Data.Listの(\\)を使用
--
-- support method
-- quick sort
qsort [] = []
qsort (x:xs) = qsort [y | y <- xs, y< x] ++ x : qsort[y | y <- xs, y >= x]
-- ex.) [1,1,2,3,4,5,5,5,6,7,9] -> [1,2,3,4,5,6,7,9,1,5,5]
originalsort xs | xs /= (qsort xs) = originalsort (qsort xs)
                | otherwise = ((nub xs) ++ (xs \\ (nub xs)))
-- count number in list
cntelem x xs | elem x xs = 1 + cntelem x (deleteOne x xs)
             | otherwise = 0

-- 1.
-- 順子
shuntsu (x:xs) | elem (x+1) xs && elem (x+2) xs = nub ([[x, (x+1), (x+2)]] ++ shuntsu xs)
               | otherwise = shuntsu xs
shuntsu [] = []
-- 刻子
kotsu (x:xs) | cntelem x xs >= 2 = [replicate 3 x] ++ kotsu (deleteOne x (deleteOne x xs))
             | otherwise = kotsu xs
kotsu [] = []
-- 対子
toitsu (x:xs) | elem x xs = [replicate 2 x] ++ toitsu (deleteOne x xs)
              | otherwise = toitsu xs
toitsu [] = []

-- 2.
-- 順子と刻子のカウント
countpair xs | kotsu (take 3 (qsort xs)) /= [] = 1 + countpair (myDrop 3 (qsort xs))
            | shuntsu (take 3 (originalsort xs)) /= [] = 1 + countpair (myDrop 3 (originalsort xs))
            | otherwise = 0

-- 対子のパターンを用意し，対子をはぶいた手で役が4つ成立しているか確認
judge xs ys | (countpair (ys \\ concat (take 1 xs))) == 4 = True
             | xs == [] = False
             | otherwise = judge (myDrop 1 xs) ys
-- 清一色
chinitsu xs = judge (toitsu xs) xs


-- 6.問3.1.3
data Tree a = Empty | Branch (Tree a) a (Tree a)
                            deriving (Eq, Ord, Show)
tree4 = Branch (Branch Empty 1 Empty) 2 (Branch Empty 3 (Branch Empty 4 Empty))
preorder  :: Tree a -> [a]
preorder Empty = []
preorder (Branch a b c) = [b] ++ (preorder a) ++ (preorder c)
inorder   :: Tree a -> [a]
inorder Empty = []
inorder (Branch a b c) = (inorder a) ++ [b] ++ (inorder c)
postorder :: Tree a -> [a]
postorder Empty = []
postorder (Branch a b c) = (postorder a) ++ (postorder c) ++ [b]


-- 7.問3.7.1
data MyList a = MyNil | MyCons a (MyList a)
instance Eq a => Eq (MyList a) where
  MyNil == MyNil = True
  MyCons a1 b1 == MyCons a2 b2 = a1 == a2 && b1 == b2
  _ == _ = False

instance Ord a => Ord (MyList a) where
  MyNil <= _ = True
  _ <= MyNil = False
  MyCons a1 b1 <= MyCons a2 b2 | a1 == a2 = b1 <= b2
                               | otherwise = a1 <= a2

toMyList :: [a] -> MyList a
toMyList [] = MyNil
toMyList (x:xs) = MyCons x (toMyList xs)
