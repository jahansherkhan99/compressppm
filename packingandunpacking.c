/**************************************************************
 *
 *                     packingandunpacking.c
 *
 *     Assignment: Arith
 *     Authors:  Jahansher Khan (jkhan03), Eashan Bagia (ebagia01)
 *     Date:     10/29/21
 *
 *     This file calls the bitpacking functions andnhandles
 *     the packing and unpacking implementation into bits
 *     for compression and decompression based on the 
 *     provided global variables.
 *
 **************************************************************/



#include "packingandunpacking.h"

/* Global constants for the widths and LSB for each variable */ 
const unsigned A_Width = 6;
const unsigned BCD_Width = 6;
const unsigned PBPR_Width = 4;
const unsigned A_lsb = 26;
const unsigned B_lsb = 20;
const unsigned C_lsb = 14;
const unsigned D_lsb = 8;
const unsigned PB_lsb = 4;
const unsigned PR_lsb = 0;


/*
 * convert_code
 *
 * Purpose: The purpose of this function is to pack everything into a 32 bit
 * word using the bitpacking implementation created. The pointer which is a
 * uint64_t is set to the codeword
 *
 * Returns: N/A
 * 
 * Parameters: column, row, pixels represented as a UArray2, elem pointer
 * and the closure
 * 
 * Expectations: Pixels, elem and closure are not NULL
 */
void convert_code(int i, int j, A2Methods_UArray2 pixels, void *elem, void *cl) 
{
    /* Assert to make sure pixels, element and closure are not NULL */ 
    assert(pixels != NULL);
    assert(elem != NULL);
    assert(cl != NULL);
    Chroma_values new_elem = elem;

    /* Get the code for each provided variable calling the Bitpack new
    functions */ 
    uint64_t code = 0;
    A2Methods_T methods = uarray2_methods_plain;
    code = Bitpack_newu(code, A_Width, A_lsb, new_elem->a);
    code = Bitpack_news(code, BCD_Width, B_lsb, new_elem->b);
    code = Bitpack_news(code, BCD_Width, C_lsb, new_elem->c);
    code = Bitpack_news(code, BCD_Width, D_lsb, new_elem->d);
    code = Bitpack_newu(code, PBPR_Width, PB_lsb, new_elem->avg_pb);
    code = Bitpack_newu(code, PBPR_Width, PR_lsb, new_elem->avg_pr);
    A2Methods_UArray2 temp = cl;   
    /* Create a temporary pointer to use the methods->at */ 
    uint64_t *ptr = methods->at(temp, i, j);
    /* Set it equal to code to get the code word */ 
    *ptr = code;
    (void) pixels;

}

/*
 * reverse_code
 *
 * Purpose: This function does the opposite: it will unpack all the code words
 * into local variables of new_elem using the bipacking implementation. It is
 * set to a new_elem which is represented by a struct
 *
 *
 * Returns: N/A
 * 
 * Parameters: column, row, pixels represented as a UArray2, elem pointer
 * and the closure
 * 
 * Expectations: Pixels, elem and closure are NULL
 */
void reverse_code(int i, int j, A2Methods_UArray2 pixels, void *elem, void *cl) 
{
    /* Ensure pixels, element and closure are not NULL */ 
    assert(pixels != NULL);
    assert(elem != NULL);
    assert(cl != NULL);
    struct Chroma_values *new_elem = malloc(sizeof(*new_elem));
    assert(new_elem != NULL);
    A2Methods_T methods = uarray2_methods_plain;
    /* Use Bitpack get functions to get the code word and set it to 
    new_elem variable */ 
    new_elem->a = Bitpack_getu(*(uint64_t *)elem, A_Width, A_lsb);
    new_elem->b = Bitpack_gets(*(uint64_t*)elem, BCD_Width, B_lsb);
    new_elem->c = Bitpack_gets(*(uint64_t*)elem, BCD_Width, C_lsb);
    new_elem->d = Bitpack_gets(*(uint64_t*)elem, BCD_Width, D_lsb);
    new_elem->avg_pb = Bitpack_getu(*(uint64_t*)elem, PBPR_Width, PB_lsb);
    new_elem->avg_pr = Bitpack_getu(*(uint64_t*)elem, PBPR_Width, PR_lsb);
    /* Set the UArray2 to the closure to make sure we can use blocks to equal
    the new element */ 
    A2Methods_UArray2 blocks = cl;
    *(Chroma_values)methods->at(blocks, i, j) = *new_elem;
    (void) pixels;
    free(new_elem);
}
