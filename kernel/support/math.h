#ifndef MATH_H
#define MATH_H

typedef float  float32_t;
typedef double float64_t;

#define PI 3.14159268
#define TAU 2*PI
#define E 2.718
#define TAYLOR_SERIES_ITERATIONS 50
#define ERROR 0xFFFFFFFF;
#define N__PRECISION 1000000

double factorial(int t);
int fp_mod(double x, int *w);
double int_exp(double b, int p);
double exp(double pow);
double ln(double a);
double power(double base, double power);


#endif