/**************************************************************
 *
 *                     compress_or_decomp.c
 *
 *     Assignment: Arith
 *     Authors:  Jahansher Khan (jkhan03), Eashan Bagia (ebagia01)
 *     Date:     10/29/21
 *
 *     This primary file calls the functions needed to compress
 *     and decompress an image which is called in 40image.c
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
#include "trim.h"
#include "rgbcvcConversion.h"
#include "chromaCalculation.h"
#include "imagePrintRead.h"
#include "packingandunpacking.h"

/*
 * compress40
 * 
 * Purpose: Compress an image and call the neccessary functions in order to
 * read, map and free 
 *
 * Output: N/A
 *
 * Parameters: a file input
 * 
 * Expectations: The input is not NULL
 * 
 *  
 */
void compress40(FILE *input)
{
    /* Assert our input is a valid input and set methods to 
    uarray2_methods_plain */ 
    assert(input != NULL);
    A2Methods_T methods = uarray2_methods_plain;
    A2Methods_mapfun *map = methods->map_default;
    /* Call the pnm_ppm read function and trim the image if neccessary and map
    it using the conversion apply function */ 
    Pnm_ppm image = Pnm_ppmread(input, methods);
    trim(image, methods, map);
    map(image->pixels, RGB2CVC_apply, image);
    /* Create a blocks UArray2 and create a new instance using half the
    width and height */ 
    A2Methods_UArray2 blocks = methods->new(image->width/2, 
                                            image->height/2, 
                                            sizeof(struct Chroma_values));
    map(blocks, chromaValue, image);
    A2Methods_UArray2 blocks_codeword = methods->new(image->width/2, 
                                                    image->height/2, 
                                                    sizeof(uint64_t)); 
    /* Map the functions and call the print functions and free blocks 
    and code word after it is completed */ 
    map(blocks, convert_code, blocks_codeword);
    print_compressed(blocks_codeword);
    methods->free(&blocks);
    methods->free(&blocks_codeword);
    Pnm_ppmfree(&image);
    
}


/*
 * decompress40
 * 
 * Purpose: Decompress an image and call the neccessary functions in order to
 * read, map and free.
 *
 * Output: N/A
 *
 * Parameters: a file input
 * 
 * Expectations: The input is not NULL
 * 
 *  
 */

void decompress40(FILE *input) 
{
     /* Assert our input is a valid input and set methods to 
    uarray2_methods_plain */ 
    assert(input != NULL);
    Pnm_ppm image = create_image(input);
    A2Methods_T methods = uarray2_methods_plain;
    A2Methods_mapfun *map = methods->map_default;
    /* Set blocks2 to a new instance of methods using half the width and
    height */ 
    A2Methods_UArray2 blocks2 = methods->new((image->width)/2, 
                                            (image->height)/2, 
                                            sizeof(struct Chroma_values));
    map(image->pixels, reverse_code, blocks2);
    /* This is the new array that is decompressed containing the normal
    width and height */
    A2Methods_UArray2 array2 = methods->new(image->width, 
                                            image->height, 
                                            sizeof(struct YPbPr));
    /* Map the functions by doing reverse DCT and converting to RGB and
    then free the pnm_ppm and blocks2 */
    map(blocks2, reverseDCT, array2);
    methods->free(&image->pixels);
    image->pixels = array2;
    map(image->pixels, CVC2RGB_apply, image);
    Pnm_ppmwrite(stdout, image);
    Pnm_ppmfree(&image);
    methods->free(&blocks2);
}
