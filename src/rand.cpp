#include "rand.h"

// @TODO: check if possible to move all these to header

namespace {
    std::random_device seed;
}

RNG::RNG() : generator_{seed()} {}

RNG& rng_singleton()
{
    static RNG singleton;
    return singleton;
}
