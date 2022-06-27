#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pnm.h"
#include <a2methods.h>
#include <a2plain.h>
#include <assert.h>
#include "uarray2.h"
#include <compress40.h>
#include <math.h>

double diff(Pnm_ppm image1, Pnm_ppm image2);

int main (int argc, char *argv[]) 
{
    FILE *fp_image1 = NULL;
    FILE *fp_image2 = NULL;
    assert(argc == 3);
    if (*argv[1] != '-' && *argv[2] != '-') {
        fp_image1 = fopen(argv[1], "rb");
        fp_image2 = fopen(argv[2], "rb");
        assert(fp_image1 != NULL && fp_image2 != NULL);
    } else if (*argv[1] != '-' && *argv[2] == '-') {
        fp_image1 = fopen(argv[1], "rb");
        fp_image2 = stdin;
        assert(fp_image1 != NULL && fp_image2 != NULL);
    } else if (*argv[1] == '-' && *argv[2] != '-') {
        fp_image1 = stdin;
        fp_image2 = fopen(argv[2], "rb");
        assert(fp_image1 != NULL && fp_image2 != NULL);
    } else if (*argv[1] == '-' && *argv[2] == '-') {
        fprintf(stderr, "Invalid comand line\n");
        exit(EXIT_FAILURE);
    }
    A2Methods_T methods = uarray2_methods_plain;

    Pnm_ppm image1 = Pnm_ppmread(fp_image1,methods);
    Pnm_ppm image2 = Pnm_ppmread(fp_image2,methods);

    double e_value = diff(image1, image2);
    printf("%f", e_value);
    return 0;
}

double diff(Pnm_ppm image1, Pnm_ppm image2) 
{
    double small_width = 0;
    double small_height = 0;
    double large_width = 0;
    double large_height = 0;
    
    if (image1->width >= image2->width) {
        small_width = image2->width;
        large_width = image1->width;
    } else {
        small_width = image1->width;
        large_width = image2->width;
    }
    if (image1->height >= image2->height) {
        small_height = image2->height;
        large_height = image1->height;
    } else {
        small_height = image1->height;
        large_height = image2->height;
    }
    if (large_width - small_width > 1) {
        fprintf(stderr, "Dimensions are off");
        exit(EXIT_FAILURE);
    }
    if (large_height - small_height > 1) {
        fprintf(stderr, "Dimensions are off");
        exit(EXIT_FAILURE);
    }
    double total_color;
    double denom1 = (double) image1->denominator;
    double denom2 = (double) image2->denominator;
    (void) denom2;
    for (int i = 0; i < small_height; i++) {
        for (int j = 0; j < small_width; j++) {
            //printf("%d,%d",i,j);
            Pnm_rgb pixel1 = (struct Pnm_rgb*)image1->methods->at(image1->pixels, j, i);
            Pnm_rgb pixel2 = (struct Pnm_rgb*)image2->methods->at(image2->pixels, j, i);
            total_color += pow((double)pixel1->red/denom1 - (double)pixel2->red/denom1, 2)
                        + pow((double)pixel1->green/denom1 - (double)pixel2->green/denom1, 2)
                        + pow((double)pixel1->blue/denom1 - (double)pixel2->blue/denom1, 2);
        }
    }
    
    total_color /= (3.0 * small_width * small_height);
    total_color = sqrt(total_color);
    return total_color;
    
}