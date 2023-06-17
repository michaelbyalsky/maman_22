#ifndef COMPLEX_H
#define COMPLEX_H

/* libraries for complex.c */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


/* type definition for complex numbers */

typedef struct {
    double real;
    double imag;
} complex;

/* function prototypes */

void read_comp(complex* comp, double real, double imag);
void print_comp(complex* comp);
void add_comp(complex* comp1, complex* comp2);
void sub_comp(complex* comp1, complex* comp2);
void mult_comp_real(complex* comp, double real);
void mult_comp_img(complex* comp, double imag);
void mult_comp_comp(complex* comp1, complex* comp2);
void abs_comp(complex* comp);
void stop();

#endif /* COMPLEX_H */