#!/usr/bin/env python3
# euclid.py
# Glenn G. Chappell
# 2025-10-15
#
# For CS 411 Fall 2025
"""Euclid's Algorithm: GCD & LCM"""


# Lines beginning ">>>" are doctests. If a command-line interface
# is available, then these can be executed by doing the following:
#
#     python3 -m doctest euclid.py
#
# When doing the above, only the results of failing tests are printed.
# Thus, no output is GOOD. To get the results of ALL tests, add "-v"
# (for "verbose"):
#
#     python3 -m doctest euclid.py -v


import sys  # for .exit


def gcd(a, b):
    """gcd(a,b) -> greatest common divisor of integers a, b.

    Uses Euclid's Algorithm.

    >>> gcd(6, 15)
    3
    >>> gcd(12, 0)
    12
    >>> gcd(756, 360)
    36
    >>> gcd(303, 1)
    1
    >>> gcd(0, 0)
    0
    >>> gcd(167408696, 2147970825)
    91

    """
    a = abs(a)
    b = abs(b)
    while a != 0:
        a, b = b%a, a
    return b


def lcm(a, b):
    """lcm(a,b) -> least common multiple of integers a, b.

    Uses function gcd.

    >>> lcm(6, 15)
    30
    >>> lcm(12, 0)
    0
    >>> lcm(756, 360)
    7560
    >>> lcm(303, 1)
    303
    >>> lcm(0, 0)
    0
    >>> lcm(167408696, 2147970825)
    3951527416036200

    """
    a = abs(a)
    b = abs(b)
    if a == 0 and b == 0:
        return 0
    return (a // gcd(a, b)) * b  # "//" is integer division


def print_results(a, b):
    """Compute GCD & LCM of a, b; print in pretty form."""

    print(f"GCD({a}, {b}) = {gcd(a,b)}")
    print(f"LCM({a}, {b}) = {lcm(a,b)}")


# main
# If this module is executed as a program (instead of being imported):
#  input a, b; print gcd(a, b), lcm(a, b); repeat until blank line input
def main():
    while True:
        print()
        print("Given integers A and B, I will compute their GCD & LCM.")
        print()
        try:
            line = input("Enter A [blank line to exit]: ")
            if line == "": break
            a = int(line)
            line = input("Enter B [blank line to exit]: ")
            if line == "": break
            b = int(line)
        except:
            print()
            print("Please type two integers, one at each prompt")
        else:
            print()
            print_results(a, b)
        print()
        print("-" * 60)

    return 0  # Success


# If this module is executed as a program (instead of being imported):
# Call function main and exit with its return value.
if __name__ == "__main__":
    sys.exit(main())

