/* 
 * 
 * bits.c - Bit manipulation functions
 */

/* 
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
         
/*#include <stdio.h>*/

int bitAnd(int x, int y) {
/*
This solution uses the negation of DeMorgan's Law: ~(p&q) = ~p OR ~q
*/
  return ~(~x | ~y);
}

/* 
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
/*

the bits are organized as B3 B2 B1 B0
we want to extract byte n from word x [example: extract B1]

--> Solution: 

We will move the digits n bytes to the right, then extract the last byte which will be n.

1 byte = 8 bits = 2 hex digits, so
Shift n by 8 to get byte's bits. 

Since:
    n << x == n * 2^x
We do:
    n << 3

Example:

Shift 12345678 right arithmetically by 2^3, or 00001000:

11123456

Now our desired byte is at the end of the number
& 0xff is an idiom that allows you to extract the last byte 
[FF == 00000000000000000000000011111111, masks everything but the last 8 bits when using & operator]

*/
    
  return (x >> (n << 3)) & 0xff;
}


/* 
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3 
 */
int logicalShift(int x, int n) {

/* 

We want to perform a logical shift (add 0s to the left) rather than the automatic arithmetic shift 
(which adds digits that corresponds to the sign).

----------
 
--> Solution:

1. Compute a regular right shift by n
2. Create a mask of 0s n times, followed by all 1s
        -> final code that does this: ~((1 << 31) >> n << 1)
3. Use & operand to compare #1 and #2 - will get 0000 * n as leading digits

Example:

n == 4

We have 0x87654321 = 1000 0111 0110 0101 0100 0011 0010 0001
We want 0x08765432 = 0000 1000 0111 0110 0101 0100 0011 0010

[since 0 <= n <= 31, we can safely shift by n]

x == 1000 0111 0110 0101 0100 0011 0010 0001

(x >> n) --> 1111 1000 0111 0110 0101 0100 0011 0010 [0x18765432]

--> Achieve step 2:

1 == 0000 0000 0000 0000 0000 0000 0000 0001 

1 << 31 --> 1000 0000 0000 0000 0000 0000 0000 0000

(1 << 31) >> n --> 1111 1000 0000 0000 0000 0000 0000 0000

((1 << 31) >> n) << 1 --> 1111 0000 0000 0000 0000 0000 0000 0000

~((1 << 31) >> n) << 1) --> 0000 1111 1111 1111 1111 1111 1111 1111

Now, we will compute the arithmetic shift & mask, to keep the 0s as leading n bits, the rest the same

(x >> n) ^ (((x & (1 << 31)) >> n) << 1) --> 

1111 1000 0111 0110 0101 0100 0011 0010
&
0000 1111 1111 1111 1111 1111 1111 1111
=
0000 1000 0111 0110 0101 0100 0011 0010 [our desired result!]

    */
    
  return (x >> n) & ~((1 << 31) >> n << 1);

}

/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {

    /*
    since going through every bit leads to too many operations,
    we'll  split the 32 bits in 4 byte chunks, count separately, and finally add
        */
        
    /*
    1:      0000 0000 0000 0000 0000 0000 0000 0001 
    1 << 8: 0000 0000 0000 0000 0000 0001 0000 0000
    1 << 16: 0000 0000 0000 0001 0000 0000 0000 0000
    1 << 24: 0000 0001 0000 0000 0000 0000 0000 0000 
    
    masks: 0000 0001 0000 0001 0000 0001 0000 0001
    */
    int masks = 1 | (1 << 8) | (1 << 16) | (1 << 24);
    int maskEnd = 0xFF; /* 0000 0000 0000 0000 0000 0000 1111 1111 */
    
    /*
    use the masks to reach the 8 bits of each byte,
    mask them with 0xFF at the end to also reach byte 0, and finally add all bits    
    */
    int sumMasks = (x & masks) + ((x >> 1) & masks) + ((x >> 2) & masks) + ((x >> 3) & masks) + 
        ((x >> 4) & masks) + ((x >> 5) & masks) + ((x >> 6) & masks) + ((x >> 7) & masks);
    int sum = (sumMasks & maskEnd) + ((sumMasks >> 8) & maskEnd) + ((sumMasks >> 16) & maskEnd) + ((sumMasks >> 24) & maskEnd);
    
    return sum;
}


/* 
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
    
/* if the number has 1s, they should all change to 0,
    if the number is all 0s, it should become 000001 */
    
    int sign = (x >> 31); /* depending on sign, this will fill with 0 or 1 bits on the left side */
    
    /* flip sign of x, and pad the left side based on sign */
    int oppositeSign = ((~x + 1) >> 31); 
    
/* 
The 2 variables will be the same only if the original sign was 0 (negating 0 yields 0): 
~0 + 1 == 0 

As per DeMorgan's Law, ~(p | q) yields: ~p & ~ q

So if x was 0, the function returns 1 (~0 & ~0 == 1)
If x was not 0, ~1 & ~0 evaluates to 0, so it returns 0
*/

	return ((~(sign | oppositeSign)) & 1);
    
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  
/*
1 << 31 == 1000 0000 0000 0000 0000 0000 0000 0000 == smallest possible number that can be represented
*/

  return 1 << 31;
    
}
/* 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
    
    /*
--> Solution:
    Shift x by given n, undo, check if it remains x - means it can be represented in n bits
    
    x << (33 - n)) >> (33 - n) equals x
    33 + ~n translates to  33 - n (since we cannot use the - operator)
    
    use the exclusive OR to check if x is still x -
    if it is, will yield 0, negate and return 1
    if it's not, will yield 1, negate and return 0
    */
    
    int doesItfit = !(((x << (33+~n)) >> (33+~n)) ^ x);
    return doesItfit;
}

/* 
 * divpwr2 - Compute x/(2^n), for 0 <= n <= 30
 *  Round toward zero
 *   Examples: divpwr2(15,1) = 7, divpwr2(-33,4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int divpwr2(int x, int n) {
    
/*
For positive numbers, x >> n yields x / 2^n [using a logical shift]
But for negative numbers, to get x/(2^n), you need: (x + 1 << n - 1) >> n
*/
    
  int negNumber = x >> 31;  /* get sign bits to check if it's pos or neg */
    
  int mask = ((negNumber & 1) << n) + negNumber; /* computes formulas above - mask will be 0 for pos numbers, 1 for negs */
    
  return (x + mask) >> n;

}

/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
    
/*
Inverting all bits and adding 1 (~x + 1) gives you the negative
*/
  return ~x + 1;
}
/* 
 * isPositive - return 1 if x > 0, return 0 otherwise 
 *   Example: isPositive(-1) = 0.
             isPositive(0) = 0
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 3
 */
int isPositive(int x) {
    
    int signX = x >> 31; /* expand on the left based on the sign bit */
    
    /*
    This statement returns 0 if the sign is 1 (negative), and 1 if the sign is 0 (non-negative)
    Uses the exclusive OR, returns either the opposite of the sign, 
    or the nagtion of it - covers the 0 case, since negating 0 turns it into 1, but a non-zero number becomes 0
    */
    return (!(signX)) ^ (!x);
}

/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */

int isLessOrEqual(int x, int y) {
/*
--> Solution:

We will use the sign bits to determine which number is greater 

Need to consider 3 cases:
The signs and values may be equal
The signs of x and y may be equal, 
Or they may be opposite. 
[negative == 1 vs non-negative == 0]
*/

/*
Create variables that store each of their signs,
and define the different possible cases
*/

    int signX = x >> 31;
    int signY = y >> 31;
    
/*
case 0: signs and values are equal
*/
    int equal = !(x^y);
    
/*
case 1: signs are equal
subcase 1: x is larger [sign of y-x, or (~y + x) is 0]
subcase 2: y is larger [sign of y-x, or (~y + x) is 1]
*/
    int xyRel = (~y + x) >> 31;
    int sameSign = (!(signX ^ signY)) & xyRel;
/*
case 2: signs are different
*/
    int diffSign = signX & (!signY);
    
    return (equal | sameSign | diffSign);
}

/*
 * ilog2 - return floor(log base 2 of x), where x > 0
 *   Example: ilog2(16) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 90
 *   Rating: 4
 */
int ilog2(int x) {
    
    /*

    Basically we need to find the most significant 1 bit in x,
    or the number of times x can be divided by 2 (before becoming less than 2)
    Since integer division by 2 is just shifting right by 1
    
    Similarly to the bitCount function, let's use a divide and conquer approach  
    with bytes B2 B1 B1 B0
    
    */

    int count1, count2, count3, count4, count5, finalCount;
        
    /* 
    Create masks to find the bits
    */
    
     /*
     Masks for B0
     
    0xF0 = 0000 0000 0000 0000 0000 0000 1111 0000
    240
    */
      int b01 = 0xF0;
    
    /*
    0x0C = 0000 0000 0000 0000 0000 0000 0000 1100
    */    
      int b02 = 0x0C;
    
    /*
    0x02 = 0000 0000 0000 0000 0000 0000 0000 0010
    */
      int b03 = 0x02;
    
    /*
    Mask for B1
    
    0xFF =     0000 0000 0000 0000 0000 0000 1111 1111
    0xFF<<8 =  0000 0000 0000 0000 1111 1111 0000 0000
    */
      int byteOne = 0xFF << 8;
    
   /*
   Masks for B2 & B3
   
    0xFF =     0000 0000 0000 0000 0000 0000 1111 1111
    0xFF<<24 = 1111 1111 0000 0000 0000 0000 0000 0000
                +
    0xFF<<16 = 0000 0000 1111 1111 0000 0000 0000 0000
    
    bytesTwoThree = 1111 1111 1111 1111 0000 0000 0000 0000
    */
      int bytesTwoThree = (0xFF << 24) + (0xFF << 16);
    
    /* !! ensures we catch both a 1 and a number with 1s in it
    to return a 1 or 0 every time 
    
    Guiding example:
    x = 0000 0000 0000 0000 0000 0000 0000 1000
    answer should be: 3
    
    (!!(x & bytesTwoThree)) << 4
    0 << 4
    
    count1:
    0000 0000 0000 0000 0000 0000 0000 0000
    */
      count1 = (!!(x & bytesTwoThree)) << 4;
      x = x >> count1;
    
    /*
    count2:
    
    (!!(x & byteOne)) << 3
    
    0000 0000 0000 0000 0000 0000 0000 1000
    &
    0000 0000 0000 0000 1111 1111 0000 0000
    =
    0000 0000 0000 0000 0000 0000 0000 0000
    << 3
    0000 0000 0000 0000 0000 0000 0000 0000
    
    no bits in byte1
    */
    
      count2 = (!!(x & byteOne)) << 3;
      x = x >> count2;
    
    /*
    count3:
    
    (!!(x & b01)) << 2
    
    0000 0000 0000 0000 0000 0000 0000 1000
    &
    0000 0000 0000 0000 0000 0000 1111 0000
    =
    0000 0000 0000 0000 0000 0000 0000 0000
    */
      count3 = (!!(x & b01)) << 2;
      x = x >> count3;
    
    /*
    count4:
    
    x & b02:
    0000 0000 0000 0000 0000 0000 0000 1000
    &
    0000 0000 0000 0000 0000 0000 0000 1100
    =
    0000 0000 0000 0000 0000 0000 0000 1000  
    
    !!(x & b02):
    0000 0000 0000 0000 0000 0000 0000 0001
    
    1 << 1:
    0000 0000 0000 0000 0000 0000 0000 0010
    
    x >> count4:
    0000 0000 0000 0000 0000 0000 0000 0011
    
    */
      count4 = (!!(x & b02)) << 1;
      x = x >> count4;
    
    /*
    count5:
    
    0100 0101 0000 0000 0000 0000 0000 0000
    &
    0000 0000 0000 0000 0000 0000 0000 0010
    =
    0000 0000 0000 0000 0000 0000 0000 0000   
    */
      count5 = (!!(x & b03));

    /*
    yields 3, correct answer since 2^3 = 8
    */
      finalCount = count1 + count2 + count3 + count4 + count5;
    
      return finalCount;
    
}
