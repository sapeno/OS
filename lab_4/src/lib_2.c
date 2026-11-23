#include "libs.h"

int gcd(int a, int b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;

    if (a == 0) return b;
    if (b == 0) return a;

    int minimum = a < b ? a : b;
    int best = 1;
    for (int i = 1; i <= minimum; i++) {
        if (a % i == 0 && b % i == 0) best = i;
    }

    return best;
}

float area(float a, float b) {
    return (a * b ) / 2.0;
}