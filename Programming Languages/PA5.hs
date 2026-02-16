-- PA5.hs  
-- Jonathan Brough
-- 2025-03-26
--
-- For CS 331 Spring 2025
-- Solutions to Assignment 5 Exercise B

module PA5 where

-- =====================================================================

-- collatzCounts
collatzCounts :: [Integer]
collatzCounts = counts
  where
    counts = map collatz [1..]
    collatz 1 = 0
    collatz n 
      | even n    = 1 + counts !! ((n `div` 2) - 1)
      | otherwise = 1 + counts !! ((3 * n + 1) - 1)

-- =====================================================================

-- operator >#
(>#) :: Ord a => [a] -> [a] -> Int
[] ># _ = 0
_ ># [] = 0
(x:xs) ># (y:ys) = (if x > y then 1 else 0) + (xs ># ys)

-- =====================================================================

-- superfilter
superfilter :: (a -> Bool) -> [a] -> [b] -> [b]
superfilter _ [] _ = []
superfilter _ _ [] = []
superfilter p (x:xs) (y:ys)
  | p x       = y : superfilter p xs ys
  | otherwise = superfilter p xs ys

-- =====================================================================

-- listSearch
listSearch :: Eq a => [a] -> a -> Maybe Int
listSearch xs val = searchHelper xs val 0
  where
    searchHelper [] _ _ = Nothing
    searchHelper (x:xs) v i
      | x == v    = Just i
      | otherwise = searchHelper xs v (i + 1)

-- =====================================================================

-- alternatingSums
alternatingSums :: Num a => [a] -> (a, a)
alternatingSums xs = foldl step (0, 0) (zip [0..] xs)
  where
    step (evenSum, oddSum) (i, x)
      | even i    = (evenSum + x, oddSum)
      | otherwise = (evenSum, oddSum + x)
