/**************************************************************
 *
 *                     chromaCalculation.h
 *
 *     Assignment: Arith
 *     Authors:  Jahansher Khan (jkhan03), Eashan Bagia (ebagia01)
 *     Date:     10/29/21
 *
 *     Contains the interface for chroma values which contains 
 *     the struct Chroma values that stores the neccessary
 *     values
 *    
 *
 **************************************************************/




#ifndef CHROMA_INCLUDED
#define CHROMA_INCLUDED

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
#include "trim.h"
#include "rgbcvcConversion.h"


typedef struct Chroma_values {
    unsigned avg_pb;
    unsigned avg_pr;
    unsigned a;
    signed b;
    signed c;
    signed d;
} *Chroma_values;

/* Main Functions */
void chromaValue(int i, int j, A2Methods_UArray2 small_array, void *elem, 
                 void *cl);
void reverseDCT(int i, int j, A2Methods_UArray2 small_array, void *elem, 
                void *cl);
float reverse_quantization_a(unsigned number);
float reverse_quantization_bcd(signed number);

/* Helper Functions */
signed convert_to_int(float var);
unsigned convert_to_int_a(float var);
void set_elem(struct YPbPr *element, float pb, float pr);
void set_range_y(float *var);


#endif
