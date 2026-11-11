#pragma once

#include <cmath>

#include "pixelengine/animation.hpp"
#include "pixelengine/color.hpp"

namespace pixelengine {

class Wave : public Animation {
   public:
    Wave(size_t led_count, uint32_t color, float wavelength = 10.0f, float amplitude = 1.0f)
        : color_(color), wavelength_(wavelength <= 0.0f ? 1.0f : wavelength),
          amplitude_(amplitude) {
        led_count_ = led_count;
        buffer_.assign(led_count_, 0);
    }

    void update() override {
        if (led_count_ == 0) {
            return;
        }
        phase_ += speed * 0.1f;
        constexpr float kTwoPi = 6.28318530718f;
        for (size_t i = 0; i < led_count_; ++i) {
            float x;
            if (layout_ == Layout::Ring) {
                x = (kTwoPi * static_cast<float>(i)) / static_cast<float>(led_count_);
                x = (x * static_cast<float>(led_count_) / wavelength_) + phase_;
            } else {
                x = (kTwoPi * static_cast<float>(i) / wavelength_) + phase_;
            }
            const float value = (std::sinf(x) * 0.5f + 0.5f) * amplitude_;
            buffer_[i] = Color::scale(color_, value);
        }
    }

   private:
    float phase_ = 0.0f;
    uint32_t color_ = Color::Blue;
    float wavelength_ = 10.0f;
    float amplitude_ = 1.0f;
};

}  // namespace pixelengine
