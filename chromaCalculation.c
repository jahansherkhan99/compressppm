/**************************************************************
 *
 *                     chromaCalculation.c
 *
 *     Assignment: Arith
 *     Authors:  Jahansher Khan (jkhan03), Eashan Bagia (ebagia01)
 *     Date:     10/29/21
 *
 *     This file contains the implementation of Chroma Calculation
 *     interface and uses the neccessary helper function 
 *     for conversions, averages and DCT.
 *    
 *
 **************************************************************/


#include "chromaCalculation.h"

/*
 * Chroma_values
 * 
 * Returns: N/A
 *
 * Purpose: Maps over a chroma values array and creates a Chroma Values struct
 * using values from 2x2 blocks within the larger array which is sent as a 
 * closure argument
 * 
 * Parameters: column, row, Array to be mapped over, element,
                closure argument which contains a PPM image
 * 
 * Expectations: Small Array, element and closure arguments are not NULL.
 *               C.R.E otherwise
 * 
 *  
 */

void chromaValue(int i, int j, A2Methods_UArray2 small_array, void *elem,
                void *cl) 
{
    /* Make sure small_array, element and closure are not NULL */
    assert(small_array != NULL);
    assert(elem != NULL);
    assert(cl != NULL);
    /* Malloc a Chroma_values struct of size *new elem */
    struct Chroma_values *new_elem = malloc(sizeof(*new_elem));
    assert(new_elem != NULL);
    Pnm_ppm image = cl;
    A2Methods_T methods = uarray2_methods_plain;
    /* Change the columns and rows so they are twice the values of the
    parameters */
    int col_large = i * 2;
    int row_large = j * 2;
    
    /* Set each element in the YPbPr struct to the appropriate part
    in the 2x2 box */
    YPbPr elem1 = methods->at(image->pixels, col_large, row_large); 
    YPbPr elem2 = methods->at(image->pixels, col_large+1, row_large); 
    YPbPr elem3 = methods->at(image->pixels, col_large, row_large+1); 
    YPbPr elem4 = methods->at(image->pixels, col_large+1, row_large+1); 

    /* Find the averages of Pb and Pr */
    float avg_pb = (elem1->pb + elem2->pb + elem3->pb + elem4->pb)/4.0;
    float avg_pr = (elem1->pr + elem2->pr + elem3->pr + elem4->pr)/4.0;
    
    /* Call the index of chroma function given to us to set it equal 
    to the new element */
    new_elem->avg_pb = (unsigned)Arith40_index_of_chroma(avg_pb);
    new_elem->avg_pr = (unsigned)Arith40_index_of_chroma(avg_pr);

    /* Apply the DCT functions on our floats a,b,c,d */
    float a = (elem4->y + elem3->y + elem2->y + elem1->y)/4.0;
    float b = (elem4->y + elem3->y - elem2->y - elem1->y)/4.0;
    float c = (elem4->y - elem3->y + elem2->y - elem1->y)/4.0;
    float d = (elem4->y - elem3->y - elem2->y + elem1->y)/4.0;
    
    /* Utilize the helper functions to convert each of the floats to int */
    new_elem->a = convert_to_int_a(a);
    new_elem->b = convert_to_int(b);
    new_elem->c = convert_to_int(c);
    new_elem->d = convert_to_int(d);
    
    /* Set it equal to new element and then free */
    *(Chroma_values)elem = *new_elem;
    free(new_elem);
    (void) small_array;

}

/*
 * convert_to_int
 * 
 * Returns: Returns a signed integer
 * Purpose: If the float passed in is greater than 0.3 or less than -0.3,
 * then set it equal to whichever of the two boundaries it's closest to
 * numerically
 * 
 * Parameters: float variable (b, c, or d) to be converted
 * 
 * Expectations: All floats are converted to some semblance of a signed int
 * 
 *  
 */
signed convert_to_int(float var)
{
    /* If anything is over 0.3 as provided by the spec, we can 
    round it down to 0.3 */
    if (var > 0.3) {
        var = 0.3;
    }
    /* Anything less than 0.3 we wil round up to 0.3 */
    else if (var < -0.3) {
        var = -0.3;
    }
    /* To scale properly for +15 and -15 we will multiply by 50 */
    signed quant = round(var * 103.3);
    return quant;
}


/*
 * reverseDCT
 * 
 * Purpose: Maps over a small chroma values array and converts each struct
 * which represents a 2x2 block to four YPbPr structs. 
 * Parameters: col, row, A2Methods_UArray2 small_array, Chroma values struct,
               larger Array of size YPbPr
 * 
 * Expectations: small_array is not NULL, there are no NULL elements and 
 * the closure argument is not NULL
 * 
 *  
 */
void reverseDCT(int i, int j, A2Methods_UArray2 small_array, void *elem,
                void *cl)
{
    /* Assert to make sure small_array, element and closure is NULL */
    assert(small_array != NULL);
    assert(elem != NULL);
    assert(cl != NULL);
    /* Adjust so the columns and rows are twice the size */
    int col_large = i * 2;
    int row_large = j * 2;

    A2Methods_T methods = uarray2_methods_plain;
    Chroma_values element = elem;
    A2Methods_UArray2 large_array = cl;

    /* Malloc each element in the YPbPr struct */
    struct YPbPr *elem1 = malloc(sizeof(*elem1));
    assert(elem1 != NULL);
    struct YPbPr *elem2 = malloc(sizeof(*elem2));
    assert(elem2 != NULL);
    struct YPbPr *elem3 = malloc(sizeof(*elem3));
    assert(elem3 != NULL);
    struct YPbPr *elem4 = malloc(sizeof(*elem4));
    assert(elem4 != NULL);

    /* Call the reverse quantization functions a,b,c,d and the 
    chroma of index function provided to us  */
    float a = reverse_quantization_a(element->a);
    float b = reverse_quantization_bcd(element->b);
    float c = reverse_quantization_bcd(element->c);
    float d = reverse_quantization_bcd(element->d);
    float pb = Arith40_chroma_of_index(element->avg_pb);
    float pr = Arith40_chroma_of_index(element->avg_pr);
    elem1->y = a - b - c + d;
    /* Call set_range_y and set_elem for each elements to ensure 
    there are no rounding issues and everthing is in range */
    set_range_y(&elem1->y);
    set_elem(elem1, pb, pr);
    elem2->y = a - b + c - d;
    set_range_y(&elem2->y);
    set_elem(elem2, pb, pr);
    elem3->y = a + b - c - d;
    set_range_y(&elem3->y);
    set_elem(elem3, pb, pr);
    elem4->y = a + b + c + d;
    set_range_y(&elem4->y);
    set_elem(elem4, pb, pr);
    
    /* Fill each part of the 2x2 box with the proper element and then free
    all of the elements */
    *(YPbPr)methods->at(large_array, col_large, row_large) = *(YPbPr)elem1;
    *(YPbPr)methods->at(large_array, col_large+1, row_large) = *(YPbPr)elem2;
    *(YPbPr)methods->at(large_array, col_large, row_large+1) = *(YPbPr)elem3;
    *(YPbPr)methods->at(large_array, col_large+1, row_large+1) = *(YPbPr)elem4;
    free(elem1);
    free(elem2);
    free(elem3);
    free(elem4);
    (void) small_array;
}

/*
 * convert_to_int_a
 * 
 * Returns: Returns an unsigned integer
 * Purpose: If the float passed in is greater than 1 or less than 0,
 * then set it equal to whichever of the two boundaries it's closest to
 * numerically
 * 
 * Parameters: float variable, a, to be converted
 * 
 * Expectations: All floats are converted to some semblance of a unsigned int
 * 
 *  
 */
unsigned convert_to_int_a(float var) {
    /* If the float is more than 1 we round to 1, if it is negative we round up 
    to 0 */
    if (var > 1.0) {
        var = 1.0;
    } else if (var < 0) {
        var = 0.0;
    }
    /* Round a by 511 which is the number provided to us*/
    unsigned quant = round(var * 63);
    return quant;
}    
/*
 * reverse_quantization_a
 * 
 * Returns: Returns a float
 * Purpose: Reverse scale by 511 and return the float. Check bounds to ensure
 * that the final value to be returned is within 0 and 1.0
 * 
 * Parameters: an unsigned integer
 * 
 * Expectations: All unsigned ints are converted to a float
 * 
 */
float reverse_quantization_a(unsigned number)
{
    /* Scale the number by 511 which was provided to us and ensure rounding
    for any values no from 0-1 */
    float final_a = number/63.0;
    if (final_a > 1.0) {
        final_a = 1.0;
    } else if (final_a < 0) {
        final_a = 0.0;
    }
    return final_a;
}
/*
 * reverse_quantization_bcd
 * 
 * Returns: Returns a float
 * Purpose: Reverse scale by 50 and return the float. Check bounds to ensure
 * that the final value to be returned is within bounds -0.3 and 0.3
 * 
 * Parameters: an signed integer
 * 
 * Expectations: All signed ints are converted to a float
 * 
 */
float reverse_quantization_bcd(signed number) 
{
    /* Scale the number 50 which we calculated and make sure the value 
    is in range */
    float final_bcd = number/103.3;
    if (final_bcd > 0.3) {
        final_bcd = 0.3;
    } else if (final_bcd < -0.3) {
        final_bcd = -0.3;
    }
    return final_bcd;
}
/*
 * set_elem
 * 
 * Returns: N/A
 * Purpose: Sets float pb and float pr to element->pb and element->pr 
 * in the struct
 * Parameters: A YPbPr struct, two float variables
 * 
 * Expectations: the struct array is not NULL
 * 
 */
void set_elem(struct YPbPr *element, float pb, float pr)
{
    /* Set pb and pr to the element in YnPbPr */
    assert(element != NULL);
    element->pb = pb;
    element->pr = pr;
 
}
/*
 * set_range_y
 * 
 * Returns: N/A
 * Purpose: Ensures the float passed in is between the range of 0 and 1
 * Parameters: A float variable pointer
 * 
 * Expectations: The float variable pointer passed in is not null
 * 
 */
void set_range_y(float *var) {
    /* Ensure the range of y is only between 0 and 1 */
    assert(var != NULL);
    if (*var > 1) {
        *var = 1;
    } else if (*var < 0) {
        *var = 0;
    }
}