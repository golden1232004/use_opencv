#ifndef __RANDOM_GENERATOR_HPP__
#define __RANDOM_GENERATOR_HPP__
#include <time.h>

class Random
{
public:
    Random ();
    Random (unsigned seed, float low = 0.0, float high = 1.0);
    float nextRand();

private:
    float m_low;
    float m_high;

};


#endif // __RANDOM_GENERATOR_HPP__
