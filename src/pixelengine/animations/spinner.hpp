#pragma once

#include <algorithm>
#include <cmath>

#include "pixelengine/animation.hpp"
#include "pixelengine/color.hpp"
#include "pixelengine/layout.hpp"

namespace pixelengine {

/** Comet / chase with a fading trail. Rings wrap; linear strips can bounce. */
class Spinner : public Animation {
   public:
    Spinner(size_t led_count, size_t trail_length, uint32_t color, bool bounce_on_linear = true)
        : trail_length_(trail_length), color_(color), bounce_on_linear_(bounce_on_linear) {
        led_count_ = led_count;
        buffer_.assign(led_count_, 0);
        if (trail_length_ > led_count_ && led_count_ > 0) {
            trail_length_ = std::max<size_t>(1, led_count_ / 2);
        }
    }

    void update() override {
        if (led_count_ == 0) {
            return;
        }
        std::fill(buffer_.begin(), buffer_.end(), 0);

        bool* rev = (layout_ == Layout::Linear && bounce_on_linear_) ? &reverse_ : nullptr;
        position_ = advancePosition(position_, speed * 0.5f, led_count_, layout_, rev);

        for (size_t i = 0; i < trail_length_; ++i) {
            float trail_pos = position_ - static_cast<float>(i);
            float wrapped = wrapFloat(trail_pos, led_count_);
            const int i1 = static_cast<int>(wrapped);
            const int i2 = static_cast<int>(wrapIndex(i1 + 1, led_count_));
            const float frac = wrapped - static_cast<float>(i1);

            float bri = static_cast<float>(trail_length_ - i) / static_cast<float>(trail_length_);
            bri *= bri;

            const uint32_t c = Color::scale(color_, bri);
            buffer_[static_cast<size_t>(i1)] =
                add(buffer_[static_cast<size_t>(i1)], Color::scale(c, 1.0f - frac));
            buffer_[static_cast<size_t>(i2)] =
                add(buffer_[static_cast<size_t>(i2)], Color::scale(c, frac));
        }
    }

    void reset() override {
        Animation::reset();
        position_ = 0.0f;
        reverse_ = false;
    }

   private:
    static uint32_t add(uint32_t a, uint32_t b) {
        const int r = std::min(255, int(Color::r(a)) + int(Color::r(b)));
        const int g = std::min(255, int(Color::g(a)) + int(Color::g(b)));
        const int bl = std::min(255, int(Color::b(a)) + int(Color::b(b)));
        return Color::rgb(uint8_t(r), uint8_t(g), uint8_t(bl));
    }

    float position_ = 0.0f;
    size_t trail_length_ = 5;
    uint32_t color_ = Color::Cyan;
    bool bounce_on_linear_ = true;
    bool reverse_ = false;
};

}  // namespace pixelengine
