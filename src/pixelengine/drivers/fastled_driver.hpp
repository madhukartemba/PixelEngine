#pragma once

#if defined(ARDUINO) && defined(FASTLED_VERSION)

#include <Arduino.h>
#include <FastLED.h>

#include <algorithm>
#include <vector>

#include "pixelengine/color.hpp"
#include "pixelengine/driver.hpp"

namespace pixelengine {

/**
 * FastLED backend. Pin must be a compile-time constant for FastLED templates;
 * this driver supports a curated set of common pins per platform.
 */
class FastLedDriver : public LedDriver {
   public:
    FastLedDriver(size_t led_count, uint8_t pin)
        : led_count_(led_count), pin_(pin), leds_(led_count) {}

    void begin() override {
        switch (pin_) {
#if defined(ESP32)
        case 2:
            FastLED.addLeds<WS2812B, 2, GRB>(leds_.data(), led_count_);
            break;
        case 4:
            FastLED.addLeds<WS2812B, 4, GRB>(leds_.data(), led_count_);
            break;
        case 5:
            FastLED.addLeds<WS2812B, 5, GRB>(leds_.data(), led_count_);
            break;
        case 18:
            FastLED.addLeds<WS2812B, 18, GRB>(leds_.data(), led_count_);
            break;
#elif defined(__AVR__)
        case 6:
            FastLED.addLeds<WS2812B, 6, GRB>(leds_.data(), led_count_);
            break;
        case 7:
            FastLED.addLeds<WS2812B, 7, GRB>(leds_.data(), led_count_);
            break;
#endif
        default:
            // Unsupported pin for this build — leave uninitialized.
            return;
        }
        pinMode(pin_, OUTPUT);
        FastLED.clear();
        FastLED.show();
        ready_ = true;
    }

    void show(const std::vector<uint32_t>& frame) override {
        if (!ready_) {
            return;
        }
        const size_t n = std::min(frame.size(), leds_.size());
        for (size_t i = 0; i < n; ++i) {
            leds_[i] = CRGB(Color::r(frame[i]), Color::g(frame[i]), Color::b(frame[i]));
        }
        FastLED.show();
    }

    size_t ledCount() const override { return led_count_; }

   private:
    size_t led_count_;
    uint8_t pin_;
    std::vector<CRGB> leds_;
    bool ready_ = false;
};

}  // namespace pixelengine

#endif
