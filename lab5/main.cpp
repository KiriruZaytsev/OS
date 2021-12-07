#include <iostream>
#include <math.h>

extern "C" int Euclid_gcd();
extern "C" int Naive_gcd();

int main() {
    int a, b;
    std::cin >> a >> b;
    int res1, res2;
    res1 = Euclid_gcd();
    res2 = Naive_gcd();
    std::cout << res1 << '\t' << res2 << '\n';
    return 0;
}
