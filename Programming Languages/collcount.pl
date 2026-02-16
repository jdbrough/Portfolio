% collcount.pl
% Jonathan Brough
% 2025-04-23
%
% Source file for Assignment 7, Exercise C

% Base case
collcount(1, 0).

% Recursive case
collcount(N, C) :-
    N > 1,
    (0 is mod(N, 2) -> Next is N // 2 ; Next is 3 * N + 1),
    collcount(Next, C1),
    C is C1 + 1.