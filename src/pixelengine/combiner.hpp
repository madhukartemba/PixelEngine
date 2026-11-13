#pragma once

#include <cstdint>
#include <vector>

#include "pixelengine/color.hpp"

namespace pixelengine {

class Combiner {
   public:
    static std::vector<uint32_t> blend(const std::vector<uint32_t>& a,
                                       const std::vector<uint32_t>& b, float t) {
        const size_t n = a.size();
        std::vector<uint32_t> out(n);
        const float u = t < 0.0f ? 0.0f : (t > 1.0f ? 1.0f : t);
        for (size_t i = 0; i < n; ++i) {
            out[i] = lerp(a[i], b[i], u);
        }
        return out;
    }

   private:
    static uint32_t lerp(uint32_t c1, uint32_t c2, float u) {
        /* Float channels — avoids uint8 underflow when fading down. */
        const float r = Color::r(c1) + (static_cast<float>(Color::r(c2)) - Color::r(c1)) * u;
        const float g = Color::g(c1) + (static_cast<float>(Color::g(c2)) - Color::g(c1)) * u;
        const float b = Color::b(c1) + (static_cast<float>(Color::b(c2)) - Color::b(c1)) * u;
        auto q = [](float v) -> uint8_t {
            if (v <= 0.0f) {
                return 0;
            }
            if (v >= 255.0f) {
                return 255;
            }
            return static_cast<uint8_t>(v + 0.5f); /* round, no temporal dither */
        };
        return Color::rgb(q(r), q(g), q(b));
    }
};

}  // namespace pixelengine
