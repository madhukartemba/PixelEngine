#pragma once

#include <algorithm>

#include "pixelengine/animation.hpp"
#include "pixelengine/color.hpp"

namespace pixelengine {

class SolidColor : public Animation {
   public:
    SolidColor(size_t led_count, uint32_t color) : color_(color) {
        is_static_ = true;
        led_count_ = led_count;
        buffer_.assign(led_count_, color_);
    }

    SolidColor(size_t led_count, uint8_t r, uint8_t g, uint8_t b)
        : SolidColor(led_count, Color::rgb(r, g, b)) {}

    void setColor(uint32_t color) {
        color_ = color;
        std::fill(buffer_.begin(), buffer_.end(), color_);
    }

    void update() override {
        for (size_t i = 0; i < led_count_; ++i) {
            buffer_[i] = color_;
        }
    }

   private:
    uint32_t color_ = Color::Black;
};

}  // namespace pixelengine
