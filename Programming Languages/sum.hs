-- sum.hs
-- Jonathan Brough
-- Simple command-line program to collect and sum user-input integers.
-- Started: 03/26/25

import System.IO  -- for flushing stdout after prompts

-- Repeatedly asks the user to input integers and totals them until a blank line is given.
accumulateSum total = do
    putStr "Input a number (or press Enter to finish): "
    hFlush stdout
    input <- getLine
    if null input then do
        putStrLn $ "You entered a total of: " ++ show total
        putStrLn ""
    else do
        let number = read input
        accumulateSum (total + number)

main = do
    putStrLn "Welcome! This program will total a list of numbers you type in."
    putStrLn "Submit one number per line. Leave a line blank to end."
    putStrLn ""
    accumulateSum 0

    putStr "Try again? (y/n): "
    hFlush stdout
    again <- getLine
    if again == "y" then do
        putStrLn ""
        main
    else
        putStrLn "Thanks for using the sum calculator!"