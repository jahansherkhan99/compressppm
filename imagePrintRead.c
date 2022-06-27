/**************************************************************
 *
 *                     imagePrintRead.c
 *
 *     Assignment: Arith
 *     Authors:  Jahansher Khan (jkhan03), Eashan Bagia (ebagia01)
 *     Date:     10/29/21
 *
 *     This files handles all of printing of a compressed 
 *     image and printing of a decompressed image in the 
 *     correct format.
 *    
 *
 **************************************************************/

#include "imagePrintRead.h"

/*
 * print_compressed
 * 
 * Returns: N/A
 *
 * Purpose: Prints out a compressed UArray2 pixels with codewords
 * Parameters: UArray2 pixels
 * 
 * Expectations: UArray2 pixels is not NULL
 * 
 *  
 */
void print_compressed(A2Methods_UArray2 pixels)
{
    /* Ensure that pixels is not NULL and store the widtrh and height  */ 
    assert(pixels != NULL);
    A2Methods_T methods = uarray2_methods_plain;
    int height = methods->height(pixels);
    int width  = methods->width(pixels);
    
    /* Print the correct header as provided by the spec  */ 
    printf("COMP40 Compressed image format 2\n%u %u", (2*width), (2*height));
    printf("\n");
        /* Utilize row major order and put everything in big endian order */ 
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
               
                uint64_t *word = methods->at(pixels, j, i);
                uint32_t pixel = (uint32_t)(*word);
                /* Big endian order starting at 24 since we have 32 bit which 
                is 4 bytes */ 
                for (int k = 24; k >= 0; k -= 8) {
                        putchar(Bitpack_getu(pixel, 8, k));
                }
            }
        }
}
/*
 * create_image
 * 
 * Returns: A Pnm_ppm image
 * Purpose: Creates a Pnm_ppm image that contains necessary information
 * to be used later in the program including a UArray2 with codewords
 *
 * Purpose: Creates an image from an input file
 * 
 * Expectations: Input file is not NULL
 * 
 *  
 */
Pnm_ppm create_image(FILE *in)
{
    assert(in != NULL);
    unsigned height, width;
    int read = fscanf(in, "COMP40 Compressed image format 2\n%u %u", &width,
               &height);
    assert(read == 2);
    int c = getc(in);
    assert(c == '\n');
    A2Methods_T methods = uarray2_methods_plain;
    A2Methods_UArray2 array = methods->new(width/2, height/2, 
                                           sizeof(uint64_t));
    /* Read in row major order since it was printed row major */
    methods->map_row_major(array, return_byte, in);
    Pnm_ppm pixmap = malloc(sizeof(*pixmap));
    assert(pixmap != NULL);
    pixmap->width = width;
    pixmap->height = height;
    pixmap->denominator = 255;
    pixmap->pixels = array;
    pixmap->methods = methods;
    return pixmap;
}
/*
 * Return_byte
 * 
 * Returns: N/A
 * Purpose: Fills in a UArray2 with codewords
 * Parameters: column, row, A2Methods_UArray2 array, element, closure
 *
 * Expectations: elem, array, closure are not NULL
 * 
 *  
 */
void return_byte(int i, int j, A2Methods_UArray2 array, void *elem, void *cl) 
{
        (void) i;
        (void) j;
        (void) array;
        assert(array != NULL);
        assert(elem != NULL);
        assert(cl != NULL);
        uint64_t byte = 0;
        FILE *file = cl;
        /* Read in 8 bytes at a time into the codewords */
        for (int k = 24; k >= 0; k -= 8) {
               int code = getc(file);
               assert(code != EOF);
               byte = Bitpack_newu(byte, 8, k, (uint64_t)code);         
        }
        uint64_t *ptr = elem;
        *ptr = byte;
}
