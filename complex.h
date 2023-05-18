#ifndef COMPLEX_H
#define COMPLEX_H

typedef struct {
    double real;
    double imag;
} complex;

void read_comp(complex* comp, double real, double imag);
void print_comp(complex* comp);
void add_comp(complex* comp1, complex* comp2);
void sub_comp(complex* comp1, complex* comp2);
void mult_comp_real(complex* comp, double real);
void mult_comp_img(complex* comp, double imag);
void mult_comp_comp(complex* comp1, complex* comp2);
void abs_comp(complex* comp);
void stop();

#define MAX_COMMAND_LENGTH 100

#endif /* COMPLEX_H */