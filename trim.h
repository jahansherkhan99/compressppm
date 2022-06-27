/**************************************************************
 *
 *                     trim.h
 *
 *     Assignment: Arith
 *     Authors:  Jahansher Khan (jkhan03), Eashan Bagia (ebagia01)
 *     Date:     10/29/21
 *
 *     This file is the interface of trim to trim and use the 
 *     new image. 
 *     
 *    
 *
 **************************************************************/


#ifndef TRIM_INCLUDED
#define TRIM_INCLUDED

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

/* Main Functions */ 
void trim(Pnm_ppm image, A2Methods_T methods, A2Methods_mapfun *map);
void txf_image(int i, int j, A2Methods_UArray2 trimmed, void *elem, void *cl);

#endif

