#include "random_generator.hpp"
#include <time.h>
#include <stdlib.h>

Random::Random():m_low(0.0), m_high(1.0)
{
    srand((unsigned)time(NULL));
}
Random::Random(unsigned seed, float low, float high):m_low(low), m_high(high)
{
    srand(seed);

}
float Random::nextRand()
{
    unsigned r = rand();
    float fr =((double) r) / RAND_MAX;
    float res = m_low + fr * (m_high - m_low);
    return res;
}
void Random::setRange(float low, float high)
{
    m_low = low;
    m_high = high;
}
unsigned Random::getMaxRandom()
{
    return RAND_MAX;
}
