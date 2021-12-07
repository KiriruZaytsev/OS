#include <iostream>

extern "C" int Euclid_gcd(int a, int b) {
    if (b == 0) {
        return a;
    } else {
        return Euclid_gcd(b, a%b);
    }
}

extern "C" int Naive_gcd(int a, int b) {
    int GCD = 1;
    for(int i = 2; i > 0; ++i) {
        if(a%i == 0 && b%i == 0) {
            GCD = i;
            break;
        }
    }
    return GCD;
}