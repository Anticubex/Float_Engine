#include "xmath.h"
#include <cstdint>

float xmath::fastInvSqrt(const float x) {
        float xhalf = 0.5f * x;
        float y = x;

        uint32_t i = *(uint32_t *)&y;   // store floating-point bits in integer
        i = 0x5f3759df - (i >> 1);      // initial guess for Newton's method
        y = *(float *)&i;               // convert new bits into float
        y = y * (1.5f - xhalf * y * y); // One round of Newton's method
        return y;
}

float xmath::precise_fastInvSqrt(const float x, const size_t rounds) {
        float xhalf = 0.5f * x;
        float y = x;

        uint32_t i = *(uint32_t *)&y; // store floating-point bits in integer
        i = 0x5f3759df - (i >> 1);    // initial guess for Newton's method
        y = *(float *)&i;             // convert new bits into float
        for (size_t i = 0; i < rounds; i++) {
                y = y * (1.5f - xhalf * y * y); // One round of Newton's method
        }
        return y;
}
