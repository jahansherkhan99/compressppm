/**************************************************************
 *
 *                     bitpack.c
 *
 *     Assignment: Arith
 *     Authors:  Jahansher Khan (jkhan03), Eashan Bagia (ebagia01)
 *     Date:     10/29/21
 *
 *     This is the implementation of bitpack that ensures
 *     bits fit properly, they can be retrieved and created.
 *     
 *    
 *
 **************************************************************/


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "compress40.h"
#include "arith40.h"
#include "uarray2.h"
#include "a2plain.h"
#include "a2methods.h"
#include "bitpack.h"
#include "assert.h"
#include "pnm.h"
#include <math.h>
#include "except.h"

Except_T Bitpack_Overflow = { "Overflow packing bits" };

/*
 * Bitpack_fitsu
 * 
 * Returns a boolean regarding whether the given unsigned integer
 * fits within the width provided
 * 
 * Purpose: Uses bit shifting manupulations to check
 * if a unsigned integer fits within the width provided
 * 
 * Parameters: A 64 bit unsigned integer, and an unsigned 32 bit width.
 * 
 * Expectations: The width is between size 0 to 64.
 */
bool Bitpack_fitsu(uint64_t n, unsigned width)
{
    assert(width <= 64 && width > 0);
    /* if bitshifting by the width makes n = 0, then it fits */
    n = n >> width;
    if (n == 0) {
        return true;
    }
    return false;
}
/*
 * Bitpack_fitsu
 * 
 * Returns a boolean regarding whether the given signed integer
 * fits within the width provided
 * Purpose: Uses bit shifting manupulations to check
 * if a signed integer fits within the width provided
 * 
 * Parameters: A 64 bit signed integer, and an unsigned 32 bit width.
 * 
 * Expectations: The width is between size 0 to 64.
 */
bool Bitpack_fitss(int64_t n, unsigned width)
{
    assert(width <= 64 && width > 0);
    
    unsigned adjusted_width = width - 1;
    /* Bitshifts left by adjusted width and finds complement */
    int64_t lowerbound = ~((int64_t)1 << adjusted_width);
    /* Bitshifts left by adjusted width for upper bound*/
    int64_t upperbound = ((int64_t)1 << adjusted_width);

    /* If it's beteen specified bounds then return true */
    if (n <= upperbound && n >= lowerbound) {
        return true;
    }
    return false; 

}
/*
 * Bitpack_getu
 * 
 * Returns an unsigned 64 bit integer extracred from the codeword which
 * is a parameter
 * Purpose: Uses masking and bit shifting manupulations to retrive
 * unsigned ints at a specific location in the codeword
 * 
 * Parameters: A 64 bit signed integer, an unsigned 32 bit width, and
 * an unsigned integer representing the least significant bit
 * 
 * Expectations: The sum of the width and lsb is less than or equal to 64,
 * and that the width itself is less than or equal to 64
 */
uint64_t Bitpack_getu(uint64_t word, unsigned width, 
                      unsigned lsb)
{
    assert((width + lsb) <= 64);
    assert(width <= 64);
    /* Here mask = 2^w, the maximum integer in this regard */
    uint64_t mask = ((uint64_t)1 << width);
    /* We subtract 1 from mask */
    mask = mask-1;
    mask = mask << lsb;

    uint64_t num = word & mask;
    num = num >> lsb;
    return num;

}
/*
 * Bitpack_gets
 * 
 * Returns a signed 64 bit integer extracred from the codeword which
 * is a parameter
 * Purpose: Uses masking and bit shifting manupulations to retrive
 * signed ints at a specific location in the codeword
 * 
 * Parameters: A 64 bit unsigned codeword, an unsigned width, and an unsigned
 * least significant bytex
 * 
 * Expectations: The sum of the width and lsb is less than or equal to 64,
 * and that the width itself is less than or equal to 64
 */

int64_t Bitpack_gets(uint64_t word, unsigned width, 
                    unsigned lsb)
{
    assert((width + lsb) <= 64);
    assert(width <= 64);
    /* Here mask = 2^w, the maximum integer in this regard */
    int64_t mask = ((int64_t)1 << width);
    mask = mask-1;
    mask = mask << lsb;
    int64_t num = word & mask;
    int64_t shift_val = 64 - (width + lsb);
    num = num << shift_val;
    int64_t new_lsb = 64 - width;
    num = num >> new_lsb;
    return num;
        
}
/*
 * Bitpack_newu
 * 
 * Returns an unsigned 64 bit codeword with a uint64_t inserted at a specified  
 * position
 * Purpose: Uses masking and bit shifting manupulations to insert
 * signed ints at a specific location in the codeword
 * 
 * Parameters: A 64 bit unsigned integer, an unsigned 32 bit width, and
 * an unsigned integer representing the least significant bit and an
 * unsigned 64 bit value to be inserted
 * 
 * Expectations: The sum of the width and lsb is less than or equal to 64,
 * and that the width itself is less than or equal to 64 and that the value 
 * fits within the given width. 
 */
   
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, 
                      uint64_t value)
{
    assert((width + lsb) <= 64);
    assert(width <= 64);
    if (!Bitpack_fitsu(value, width)) {
        RAISE(Bitpack_Overflow);
    }
    /* Here mask = 2^w, the maximum integer in this regard */
    uint64_t mask = ((uint64_t)1 << width);
    mask = mask-1;
    mask = mask << lsb;
    mask = ~mask;
    /* "And" both to create all 0's inside the word where you want 
    valeu inserted */
    uint64_t temp_word = word & mask;
    value = value << lsb;
    /* Insert word by "OR" */
    uint64_t final_word = temp_word | value;
    
    return final_word;
}
/*
 * Bitpack_news
 * 
 * Returns an unsigned 64 bit integer extracred from the codeword which
 * is a parameter
 * Purpose: Uses masking and bit shifting manupulations to insert
 *  signed ints at a specific location in the codeword
 * 
 * Parameters: A 64 bit unsigned integer, an unsigned 32 bit width, and
 * a signed integer representing the least significant bit and an
 * unsigned 64 bit value to be inserted
 * 
* Expectations: The sum of the width and lsb is less than or equal to 64,
 * and that the width itself is less than or equal to 64 and that the value 
 * fits within the given width. 
 */
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb, 
                      int64_t value) 
{
    /* Assert and make sure width is less than 64 */
    assert((width + lsb) <= 64);
    assert(width <= 64);

    /* Raise error if needed for overflow*/
    if (!Bitpack_fitss(value, width)) {
        RAISE(Bitpack_Overflow);
    }
    /* Create a mask of 1s and subtract one and take complement of mask */
    uint64_t mask = ((uint64_t)1 << width);
    mask = mask-1;
    mask = mask << lsb;
    mask = ~mask;
    /* "And" both to create all 0's inside the word where you want 
    value inserted */
    uint64_t temp_word = word & mask;
    mask = ~mask;
    value = value << lsb;
    uint64_t temp2 = value & mask;
    /* OR the new temp word with original temp word to get the 
    correct value */
    temp2 = temp2 | temp_word;
    return temp2;
        
}      
