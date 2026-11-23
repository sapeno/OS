#include "libs.h"

int gcd(int a, int b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;

    while (b) {
        int tmp = b;
        b = a % b;
        a = tmp;
    }
    return a;
}

float area(float a, float b) {
    return a * b;
}