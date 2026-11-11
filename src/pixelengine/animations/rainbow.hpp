#pragma once

#include "pixelengine/animation.hpp"
#include "pixelengine/color.hpp"

namespace pixelengine {

class Rainbow : public Animation {
   public:
    explicit Rainbow(size_t led_count, float spd = 1.0f) {
        led_count_ = led_count;
        speed = spd;
        buffer_.assign(led_count_, 0);
    }

    void update() override {
        if (led_count_ == 0) {
            return;
        }
        hue_offset_ += static_cast<uint16_t>(speed * 256.0f);
        for (size_t i = 0; i < led_count_; ++i) {
            const uint16_t hue =
                static_cast<uint16_t>(hue_offset_ + (i * 65536UL / led_count_));
            buffer_[i] = Color::hsv(hue);
        }
    }

   private:
    uint16_t hue_offset_ = 0;
};

}  // namespace pixelengine
