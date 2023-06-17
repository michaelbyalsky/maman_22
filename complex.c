/*
 * This program is a complex number calculator.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "complex.h"

void read_comp(complex* comp, double real, double imag) { /* read a complex number from the user */
    comp->real = real;
    comp->imag = imag;
    printf("read_comp: ");
    print_comp(comp);
}

void print_comp(complex* comp) { /* print a complex number */
    printf("print_comp: ");
    printf("%.2f + (%.2f)i\n", comp->real, comp->imag);
}

void add_comp(complex* comp1, complex* comp2) { /* add 2 complex numbers and print the result */
    /* print comp1 before the addition and comp2 */
    printf("add_comp: (%.2f + (%.2f)i) + (%.2f + (%.2f)i)\n", comp1->real, comp1->imag, comp2->real, comp2->imag);
    comp1->real += comp2->real;
    comp1->imag += comp2->imag;
    print_comp(comp1);
}

void sub_comp(complex* comp1, complex* comp2) { /* subtract 2 complex numbers and print the result */
    printf("sub_comp: (%.2f + (%.2f)i) - (%.2f + (%.2f)i)\n", comp1->real, comp1->imag, comp2->real, comp2->imag);
    comp1->real -= comp2->real;
    comp1->imag -= comp2->imag;
    print_comp(comp1);
}

void mult_comp_real(complex* comp, double real) { /* multiply a complex number by a real number and print the result */
    printf("mult_comp_real: (%.2f + (%.2f)i) * %.2f\n", comp->real, comp->imag, real);
    comp->real *= real;
    comp->imag *= real;
    print_comp(comp);
}

void mult_comp_img(complex* comp, double imag) { /* multiply a complex number by an imaginary number and print the result */
    comp->real *= imag;
    comp->imag *= imag;
    printf("mult_comp_img: (%.2f + (%.2f)i) * %.2f\n", comp->real, comp->imag, imag);
    print_comp(comp);
}

void mult_comp_comp(complex* comp1, complex* comp2) { /* multiply 2 complex numbers and print the result */
    double real = comp1->real * comp2->real - comp1->imag * comp2->imag;
    double imag = comp1->real * comp2->imag + comp1->imag * comp2->real;
    printf("mult_comp_comp: (%.2f + (%.2f)i) * (%.2f + (%.2f)i)\n", comp1->real, comp1->imag, comp2->real, comp2->imag);
    printf("%.2f + (%.2f)i\n", real, imag);
}

void abs_comp(complex* comp) { /* print the absolute value of a complex number */
    double abs_val = sqrt(pow(comp->real, 2) + pow(comp->imag, 2));
    printf("abs_comp: %.2f + (%.2f)i\n", comp->real, comp->imag);
    printf("The absolute value of %.2f + (%.2f)i is %.2f\n", comp->real, comp->imag, abs_val);
}

void stop() { /* stop the program */
    printf("Goodbye!\n");
    exit(0);
}
