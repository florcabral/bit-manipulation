# Bit manipulation

This is an implementation of the following functions for bit manipulation in C:

* **bitAnd** - computes x&y using only ~ and | 
* **getByte** - extracts byte n from word x
* **logicalShift** - shift x to the right by n, using a logical shift
* **bitCount** - returns count of number of 1's in word
* **bang** - computes !x without using !
* **tmin** - returns minimum two's complement integer
* **fitsBits** - returns 1 if x can be represented as an n-bit, two's complement integer
* **divpwr2** - computes x/(2^n), for 0 <= n <= 30
* **negate** - return -x
* **isPositive** - returns 1 if x > 0, return 0 otherwise
* **isLessOrEqual** - if x <= y  then return 1, else return 0
* **ilog2** - returns floor(log base 2 of x), where x > 0

## How to run and test

The Makefile in this directory compiles bits.c with
additional code to create a program (or test harness) named btest.

To compile and run the btest program, type:

    unix> make btest
    unix> ./btest [optional cmd line args]