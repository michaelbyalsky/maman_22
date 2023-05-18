#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "complex.h"

void read_comp(complex* comp, double real, double imag) {
    comp->real = real;
    comp->imag = imag;
}

void print_comp(complex* comp) {
    printf("%.2f + (%.2f)i\n", comp->real, comp->imag);
}

void add_comp(complex* comp1, complex* comp2) {
    comp1->real += comp2->real;
    comp1->imag += comp2->imag;
    print_comp(comp1);
}

void sub_comp(complex* comp1, complex* comp2) {
    comp1->real -= comp2->real;
    comp1->imag -= comp2->imag;
    print_comp(comp1);
}

void mult_comp_real(complex* comp, double real) { /* multiply a complex number by a real number and print the result */
    comp->real *= real;
    comp->imag *= real;
    print_comp(comp);
}

void mult_comp_img(complex* comp, double imag) { /* multiply a complex number by an imaginary number and print the result */
    comp->real *= imag;
    comp->imag *= imag;
    print_comp(comp);
}

void mult_comp_comp(complex* comp1, complex* comp2) { /* multiply 2 complex numbers and print the result */
    double real = comp1->real * comp2->real - comp1->imag * comp2->imag;
    double imag = comp1->real * comp2->imag + comp1->imag * comp2->real;
    printf("%.2f + (%.2f)i\n", real, imag);
}

void abs_comp(complex* comp) {
    double abs_val = sqrt(pow(comp->real, 2) + pow(comp->imag, 2));
    printf("The absolute value of %.2f + (%.2f)i is %.2f\n", comp->real, comp->imag, abs_val);
}

void stop() { /* stop the program */
    printf("Goodbye!\n");
    exit(0);
}
