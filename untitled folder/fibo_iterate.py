#!/usr/bin/env python3
# fibo_iterate.py
# Glenn G. Chappell
# 2025-09-08
#
# For CS 411 Fall 2025
"""Computing Fibonacci Numbers
Iterative"""


# Lines beginning ">>>" are doctests. If a command-line interface
# is available, then these can be executed by doing the following:
#
#     python3 -m doctest fibo_iterate.py
#
# When doing the above, only the results of failing tests are printed.
# Thus, no output is GOOD. To get the results of ALL tests, add "-v"
# (for "verbose"):
#
#     python3 -m doctest fibo_iterate.py -v


import itertools  # for .count
import sys        # for.version_info, get_int_max_str_digits,
                  #  set_int_max_str_digits

def fibo(n):
    """nonnegative int n -> F(n), the nth Fibonacci no.

    F(0) = 0. F(1) = 1. For k >= 2, F(k) = F(k-2) + F(k-1).

    Uses fibopair.

    Pre:
        n >= 0.

    >>> fibo(0)
    0
    >>> fibo(5)
    5
    >>> fibo(300)
    222232244629420445529739893461909967206666939096499764990979600

    """
    prev = 1
    curr = 0

    for i in range(n):
        # Slide prev, curr to the right by one
        prev, curr = curr, prev + curr

    return curr


def set_conversion_digits():
    """Ensure that large integers can be printed."""

    # We are using Py 3.*, of course.
    assert sys.version_info.major == 3

    # Integer -> string conversion limit handling added in Py 3.11. If
    # we are running an earlier version, then do nothing.
    if sys.version_info.minor < 11:
        return

    # Allow for integers with 1 billion digits.
    digits_needed = 1000000000
    if sys.get_int_max_str_digits() < digits_needed:
        sys.set_int_max_str_digits(digits_needed)


# main
# Prints F(i) for i = 0, 1, 2, ... and so on, forever.
# Uses fibo.
def main():
    set_conversion_digits()  # Make sure we can print large integers

    print("Fibonacci Numbers")
    print()
    for i in itertools.count(0):
        # Compute Fibonacci number
        ff = fibo(i)

        # Print it nicely
        print("F(", i, ") = ", ff, sep="")


# If this module is executed as a program (instead of being imported):
# Call function main and exit with its return value.
if __name__ == "__main__":
    sys.exit(main())

