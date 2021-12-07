#include <iostream>
#include <math.h>

extern "C" float Leibniz(int k) {
    double Pi = 0.0;
    for (int i = 0; i<k; ++i) {
        Pi += (pow(-1, i) / (2*i+1));
    }
    Pi *= 4;
    return Pi;
}

extern "C" float Wallis(int k) {
    double Pi = 1.0;
    for (int i = 1; i < k; ++i) {
        Pi *= ((4.0*i*i) / (4.0*i*i - 1.0));
    }
    Pi *= 2;
    return Pi;
}

