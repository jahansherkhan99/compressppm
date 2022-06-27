/**************************************************************
 *
 *                     rgbcvcConversion.c
 *
 *     Assignment: Arith
 *     Authors:  Jahansher Khan (jkhan03), Eashan Bagia (ebagia01)
 *     Date:     10/29/21
 *
 *     This file contains the implementation of converting 
 *     from rgb to cvc and cvc to rgb. It contains helper
 *     functions to aid in edge cases. 
 *    
 *
 **************************************************************/

#include "rgbcvcConversion.h"

/*
 * RGB2CVC
 *
 * Purpose: The purpose of this function is to convert from RGB values to 
 * the compononent video values using the formulas provided
 *
 * Returns: N/A
 * 
 * Parameters: The row, column, pixels represented as UArray2, elem pointer
 * and the closure
 * 
 * Expectations: The elem, pixels and/or closure are not NULL
 */
void RGB2CVC_apply(int i, int j, A2Methods_UArray2 pixels, 
void *elem, void *cl)
{
    /* Ensure the element, pixels and closure is not NULL and 
    void unused variables */ 
    assert(elem != NULL);
    assert(pixels != NULL);
    assert(cl != NULL);
    (void) i;
    (void) j;
    (void) pixels;
    
    /* Malloc a YPbPr struct and set the temp to a closure casted as pnm_ppm */ 
    YPbPr new_elem = malloc(sizeof(struct YPbPr));
    assert(new_elem != NULL);
    Pnm_rgb element = elem;
    Pnm_ppm temp = (Pnm_ppm)cl;
    
    /* Utilize the denominatory so RGB are scaled */ 
    float element_red = (float)element->red/temp->denominator;
    float element_green = (float)element->green/temp->denominator;
    float element_blue = (float)element->blue/temp->denominator;

    /* Perfeorm the conversion susing formulas provided and free new element */ 
    new_elem->y = 0.299 * element_red + 0.587 * element_green 
    + 0.114 * element_blue;
    new_elem->pb = -0.168736 * element_red - 0.331264 * element_green 
    + 0.5 * element_blue;
    new_elem->pr = 0.5 * element_red - 0.418688 * element_green - 
    0.081312 * element_blue;
    *(YPbPr)elem = *(YPbPr)new_elem;
    free(new_elem);
}



/*
 * CVC2RGB 
 *
 * Purpose: The purpose of this function is to convert from component video
 * values to the RGB values using the provided formulas and making sure 
 * everything is rounded within the bounds
 *
 *
 * Returns: N/A 
 * 
 * Parameters: Column, row, pixels represented as UArray2, an elem and 
 * the closure
 * 
 * Expectations: The elem, pixels and closure are not NULL
 */

void CVC2RGB_apply(int i, int j, A2Methods_UArray2 pixels, 
void *elem, void *cl) 
{
    /* Assert the element, pixels and closure and void unused parameters */ 
    assert(elem != NULL);
    assert(pixels != NULL);
    assert(cl != NULL);
    (void) i;
    (void) j;
    (void) pixels;
    /* Malloc a new element with size of the struct pnm_rgb */ 
    Pnm_rgb new_elem = malloc(sizeof(struct Pnm_rgb));
    assert(new_elem != NULL);
    YPbPr element = (YPbPr)elem;
    Pnm_ppm temp = (Pnm_ppm)cl;
    /* Using a denomiator of 255, apply this to YPbPr */ 
    float element_y = (float)element->y*255;
    float element_pb = (float)element->pb*255;
    float element_pr = (float)element->pr*255;

    /* Apply the conversions to the float values and call reinforcement
    function to check for edge cases */ 
    float redf = 1.0 * element_y + 0.0 * element_pb + 1.402 * element_pr;
    unsigned red = reinforcement_function(redf);

    float greenf = 1.0 * element_y - 0.344136 * element_pb -
    0.714136 * element_pr;
    unsigned green = reinforcement_function(greenf);

    float bluef = 1.0 * element_y + 1.772 * element_pb + 0.0 * element_pr;
    unsigned blue = reinforcement_function(bluef);

    /* Set each RGB to the newly calculated ones and free new element after */ 
    new_elem->red = red;
    new_elem->green = green;
    new_elem->blue = blue;
    *(Pnm_rgb)elem = *(Pnm_rgb)new_elem;
    free(new_elem);
    (void) temp;
}

/*
 * reinforcement function
 *
 * Purpose: This function will make sure any number more than 255 or less than 
 * 0 is rounded properly to prevent conversion issues
 *
 *
 * Returns: An unsigned number
 * 
 * Parameters: A float
 * 
 * Expectations: A proper float is provided
 */
unsigned reinforcement_function(float number) {
    /* If number is more than 255, we bring it back down to 255 */ 
    if (number > 255) {
        number = 255;
    } 
    /* All negative numbers become 0  */ 
    else if(number < 0) {
        number = 0;
    }
    return number;

}