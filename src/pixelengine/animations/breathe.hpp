#pragma once

#include <cmath>

#include "pixelengine/animation.hpp"
#include "pixelengine/color.hpp"

namespace pixelengine {

class Breathe : public Animation {
   public:
    Breathe(size_t led_count, uint32_t color, float min_brightness = 0.2f)
        : color_(color), min_brightness_(clamp01(min_brightness)) {
        led_count_ = led_count;
        buffer_.assign(led_count_, color_);
    }

    void update() override {
        if (led_count_ == 0) {
            return;
        }
        phase_ += speed * 0.05f;
        constexpr float kTwoPi = 6.28318530718f;
        if (phase_ > kTwoPi) {
            phase_ -= kTwoPi;
        }
        const float raw = std::sinf(phase_ - 1.57079632679f) * 0.5f + 0.5f;
        const float bri = min_brightness_ + (1.0f - min_brightness_) * raw;
        const uint32_t c = Color::scale(color_, bri);
        for (size_t i = 0; i < led_count_; ++i) {
            buffer_[i] = c;
        }
    }

   private:
    static float clamp01(float v) {
        if (v < 0.0f) {
            return 0.0f;
        }
        if (v > 1.0f) {
            return 1.0f;
        }
        return v;
    }

    float phase_ = 0.0f;
    uint32_t color_ = Color::White;
    float min_brightness_ = 0.2f;
};

}  // namespace pixelengine
