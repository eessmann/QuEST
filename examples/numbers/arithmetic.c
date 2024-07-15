#include "quest.h"

int main() {

    initQuESTEnv();

    // literal 1.2 + 3.4i is restrictedly double precision
    qcomp x = qcomp(1.2, 3.4);

    // C + R
    x = x + (int) 3;
    x = x + (qindex) 3;
    x = x + (float) 2;
    x = x + (double) 2;
    x = x + (long double) 2;

    // R + C
    x = (int) 3         + x;
    x = (qindex) 3      + x;
    x = (float) 2       + x;
    x = (double) 2      + x;
    x = (long double) 2 + x;

    // C - R
    x = x - (int) 3;
    x = x - (qindex) 3;
    x = x - (float) 2;
    x = x - (double) 2;
    x = x - (long double) 2;

    // R - C
    x = (int) 3         - x;
    x = (qindex) 3      - x;
    x = (float) 2       - x;
    x = (double) 2      - x;
    x = (long double) 2 - x;

    // C * R
    x = x * (int) 3;
    x = x * (qindex) 3;
    x = x * (float) 2;
    x = x * (double) 2;
    x = x * (long double) 2;

    // R * C
    x = (int) 3         * x;    
    x = (qindex) 3      * x;
    x = (float) 2       * x;
    x = (double) 2      * x;
    x = (long double) 2 * x;

    // C / R
    x = x / (int) 3;
    x = x / (qindex) 3;
    x = x / (float) 2;
    x = x / (double) 2;
    x = x / (long double) 2;

    // R / C
    x = (int) 3         / x;
    x = (qindex) 3      / x;
    x = (float) 2       / x;
    x = (double) 2      / x;
    x = (long double) 2 / x;

    // C += R
    x += (int) 3;
    x += (qindex) 3;
    x += (float) 2;
    x += (double) 2;
    x += (long double) 2;

    // C -= R
    x -= (int) 3;
    x -= (qindex) 3;
    x -= (float) 2;
    x -= (double) 2;
    x -= (long double) 2;

    // C *= R
    x *= (int) 3;
    x *= (qindex) 3;
    x *= (float) 2;
    x *= (double) 2;
    x *= (long double) 2;

    // C /= R
    x /= (int) 3;
    x /= (qindex) 3;
    x /= (float) 2;
    x /= (double) 2;
    x /= (long double) 2;

    reportQcomp(x);

    finalizeQuESTEnv();
}