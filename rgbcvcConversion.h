/**************************************************************
 *
 *                     rgbcvcConversion.h
 *
 *     Assignment: Arith
 *     Authors:  Jahansher Khan (jkhan03), Eashan Bagia (ebagia01)
 *     Date:     10/29/21
 *
 *     This file is the interface for rgbCVC that contains the 
 *     struct YPBPR which holds the float values for the 
 *     conversion functions. 
 *    
 *
 **************************************************************/


#ifndef RGB_INCLUDED
#define RGB_INCLUDED
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

typedef struct YPbPr {
    float y;
    float pb;
    float pr;
} *YPbPr;

/* Main Functions */ 
void RGB2CVC_apply(int i, int j, A2Methods_UArray2 pixels, void *elem, 
                   void *cl);
void CVC2RGB_apply(int i, int j, A2Methods_UArray2 pixels, void *elem, 
                   void *cl);

/* Helper Function */ 
unsigned reinforcement_function(float number);

#endif