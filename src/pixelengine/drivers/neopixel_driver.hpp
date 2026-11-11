#pragma once

#if defined(ARDUINO)

#include <Adafruit_NeoPixel.h>

#include <vector>

#include "pixelengine/driver.hpp"

namespace pixelengine {

class NeoPixelDriver : public LedDriver {
   public:
    NeoPixelDriver(size_t led_count, uint8_t pin, neoPixelType type = NEO_GRB + NEO_KHZ800)
        : led_count_(led_count), pin_(pin), strip_(led_count, pin, type) {}

    void begin() override {
        strip_.begin();
        strip_.show();
    }

    void show(const std::vector<uint32_t>& frame) override {
        const size_t n = std::min(frame.size(), led_count_);
        for (size_t i = 0; i < n; ++i) {
            strip_.setPixelColor(i, frame[i]);
        }
        strip_.show();
    }

    size_t ledCount() const override { return led_count_; }

   private:
    size_t led_count_;
    uint8_t pin_;
    Adafruit_NeoPixel strip_;
};

}  // namespace pixelengine

#endif  // ARDUINO
