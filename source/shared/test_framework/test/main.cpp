#include <test_framework/test_framework.hpp>

#include <cmath>

namespace m = test_framework::matchers;

float test_sqrt(float in)
{
    TEST_THAT(in, m::equals_to(0.f) || m::greater_than(0.f));
    if (in < 0)
        return NAN;

    return std::sqrt(in);
}

int main()
{
    test_sqrt(-1);
}