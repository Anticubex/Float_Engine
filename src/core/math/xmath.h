#include <cstddef>

namespace xmath {

float fastInvSqrt(const float x);
float precise_fastInvSqrt(const float x, const size_t rounds);

template <class T = size_t>
int isPow2(T num);

}; // namespace xmath
