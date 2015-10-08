#include "timer.h"

class FPSLimit {
public:
    FPSLimit() = default;
    explicit FPSLimit(int const fps) { set_limit(fps); }

    void set_limit(int const fps) { frame_duration_ms_ = 1000 / fps; }
    void enforce(const Timer& timer)
    {
        if (frame_duration_ms_ - timer.get_ticks() > min_system_sleep_) {
            SDL_Delay(1);
        }
    }

private:
    Timer::TicksT frame_duration_ms_{};
    static Timer::TicksT min_system_sleep_{10}; // @TODO: verify
};
