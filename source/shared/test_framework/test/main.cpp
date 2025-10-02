#include <test_framework/test_framework.hpp>

#include <cmath>

float test_sqrt(float in)
{
    TEST_THAT(in, equals_to(0) || greater_than(0));
    if (in < 0)
        return NAN;

    return std::sqrt(in);
}

int main()
{
    test_sqrt(-1);
}