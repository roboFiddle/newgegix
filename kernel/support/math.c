#include "math.h"

double factorial(int t) {
    if(t == 0 || t == 1)
        return 1;
    return t*factorial(t-1);
}
int mod(int x, int base) {
    while(x >= base) {
        x -= base;
    }
    return x;
}
int fp_mod(double x, int *w) {
    *w = (int)x;
    x -= (int)x;
    double f = 0;
    while(f < 10000000) {
        x *= 10;
        f *= 10;
        f += (int) x;
        x -= (int) x;
    }
    return (int)f;
}
double int_exp(double b, int p) {
    double r = 1;
    for(int i = 0; i < p; i++)
        r *= b;
    return r;
}
double exp(double pow) { // find e^p
    if(pow == 1) {
        return E;
    }
    else {
        double r = 0;
        for(int i = 0; i < TAYLOR_SERIES_ITERATIONS; i++) {
            r += (int_exp(pow, i) / factorial(i));
        }
    }
    return 0;
}
double ln(double a) {
    if(a < 0) {
        return ERROR;
    }
    else if(a == 1) {
        return 0;
    }
    else {
        a -= 1;
        double r = 0;
        for(int i = 1; i < TAYLOR_SERIES_ITERATIONS; i++) {
            if(i%2 == 0) {
                r -= (int_exp(a, i) / i);
            }
            else {
                r += (int_exp(a, i) / i);
            }
        }
    }
    return 0;
}
double power(double base, double power) { // find b^p
    return exp(power * ln(base));
}