/**************************************************************
 *
 *                     imagePrintRead.h
 *
 *     Assignment: Arith
 *     Authors:  Jahansher Khan (jkhan03), Eashan Bagia (ebagia01)
 *     Date:     10/29/21
 *
 *     This is the interface for image printing where it 
 *     is represented as a A2Method_UArray2.
 *     
 *    
 *
 **************************************************************/


#ifndef PR_INCLUDED
#define PR_INCLUDED

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
#include "packingandunpacking.h"

/* Main Functions */ 
void print_compressed(A2Methods_UArray2 pixels);
Pnm_ppm create_image(FILE *input);
void return_byte(int i, int j, A2Methods_UArray2 array, void *elem, void *cl);

#endif