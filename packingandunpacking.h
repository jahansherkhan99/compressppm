/**************************************************************
 *
 *                     packingandunpacking.h
 *
 *     Assignment: Arith
 *     Authors:  Jahansher Khan (jkhan03), Eashan Bagia (ebagia01)
 *     Date:     10/29/21
 *
 *     This is the interface for the packing and unpacking 
 *     file where both compression and decompression versions
 *     are used. 
 *    
 *
 **************************************************************/


#ifndef PAUP_INCLUDED
#define PAUP_INCLUDED

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
#include "chromaCalculation.h"

/* Main Functions */ 
void convert_code(int i, int j, A2Methods_UArray2 pixels, void *elem, void *cl);
void reverse_code(int i, int j, A2Methods_UArray2 pixels, void *elem, void *cl);

#endif