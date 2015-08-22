#include <random>

class RNG {
public:
    RNG();

    int rand_int() { return int_distribution_(generator_); }
    float rand_float() { return float_distribution_(generator_); }

private:
    std::mt19937 generator_;
    std::uniform_int_distribution<> int_distribution_;
    std::uniform_real_distribution<float> float_distribution_;
};

// All these functions have noticeable bias, but our requirements
// are decidedly low for randomness
int rand_int(RNG& rng, int const max_val) { return rng.rand_int() % max_val; }
int rand_int(RNG& rng, int const min_val, int const max_val)
    { return rand_int(rng, max_val - min_val) + min_val; }
bool rand_bool(RNG& rng) { return rng.rand_int(2); }
float rand_float(RNG& rng, float const max_val) { return rng.rand_float() * max_val; }
float rand_float(RNG& rng, float const min_val, float const max_val)
    { return rng.rand_float() * (max_val - min_val) +  min_val; }


// Utility functions
RNG& rng_singleton();

int rand_int() { return rand_int(rng_singleton()); }
int rand_int(int const max_val) { return rand_int(rng_singleton(), max_val); }
int rand_int(int const min_val, int const max_val)
    { return rand_int(rng_singleton(), min_val, max_val); }
float rand_float(float const max_val) { return rand_float(rng_singleton(), max_val); }
float rand_float(float const min_val, float const max_val)
    { return rand_float(rng_singleton(), min_val, max_val); }
bool rand_bool() { return rand_bool(rng_singleton()); }
