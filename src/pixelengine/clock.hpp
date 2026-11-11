#pragma once

#include <cstdint>

#if defined(ARDUINO)
#include <Arduino.h>
#elif defined(ESP_PLATFORM)
#include "esp_timer.h"
#endif

namespace pixelengine {

/** Millisecond clock used by the engine (Arduino millis / ESP timer). */
inline uint32_t nowMs() {
#if defined(ARDUINO)
    return millis();
#elif defined(ESP_PLATFORM)
    return static_cast<uint32_t>(esp_timer_get_time() / 1000ULL);
#else
    extern uint32_t pixelengine_host_now_ms();
    return pixelengine_host_now_ms();
#endif
}

}  // namespace pixelengine
