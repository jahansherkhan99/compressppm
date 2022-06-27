/**************************************************************
 *
 *                     trim.c
 *
 *     Assignment: Arith
 *     Authors:  Jahansher Khan (jkhan03), Eashan Bagia (ebagia01)
 *     Date:     10/29/21
 *
 *     This file contains the implenetation for trimming to ensure 
 *     the height and width don't raise issues. 
 *     
 *    
 *
 **************************************************************/


#include "trim.h"


/*
 * trim
 *
 * Purpose: This function will assert to make sure nothing is NULL
 * and then trim the image if the width and/or height or not even 
 * and map over and set it equal to a new pixels
 *
 * Returns: N/A
 * 
 * Parameters: A pnm_ppm represented image, the methods functions from 
 * A2Methods_T and a map pointer from A2Methods_mapfun
 * 
 * Expectations: The image, methods and/or map is not NULL and the image
 * is at least a 2x2 picture.
 */

void trim(Pnm_ppm image, A2Methods_T methods, A2Methods_mapfun *map) 
{
    /* Ensure the images, methods, or map is not NULL and assert 
    if neccessary*/                                                        
    assert(image != NULL);
    assert(methods != NULL);
    assert(map != NULL);

    /* If the width and height are not greater than 2, 
    we raise an error */ 
    assert(image->width >= 2 && image->height >= 2);
    /* Trim the width and/or the height if it is not an even number */ 
    if (image->width % 2 != 0) {
        image->width--;
    }
    if (image->height % 2 != 0) {
        image->height--;
    }
    A2Methods_UArray2 new_pixels = methods->new(image->width, image->height, 
                                                sizeof(struct Pnm_rgb));
    /* Map the new_pixels and free the pixels. Set the image to the new pixels 
    with modified height and/or width if needed*/ 
    map(new_pixels, txf_image, image);
    methods->free(&image->pixels);
    image->pixels = new_pixels;
}

/*
 * txf_image
 *
 * Purpose: Set the pnm_ppm to the closure and set the elem
 * to the pixel at i and j
 *
 * Returns: N/A
 * 
 * Parameters: The column, row, trimmed image in the form of a UArray2, 
 * the elem pointer and closure
 * 
 * Expectations: Elem and/or closure is not NULL
 */
void txf_image(int i, int j, A2Methods_UArray2 trimmed, void *elem, void *cl)
{
    /* Ensure the element or closure is not NULL and assert 
    if neccessary*/ 
    assert(elem != NULL);
    assert(cl != NULL);
    (void) trimmed;
    Pnm_ppm image = cl;
    *(Pnm_rgb)elem = *(Pnm_rgb)image->methods->at(image->pixels, i, j);
}