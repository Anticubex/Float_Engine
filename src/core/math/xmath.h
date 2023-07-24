#include <cstddef>

namespace xmath {

float fastInvSqrt(const float x);
float precise_fastInvSqrt(const float x, const size_t rounds);

int isPow2(size_t num);

}; // namespace xmath
