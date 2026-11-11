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
        for (size_t i = 0; i < n; ++i) {
            out[i] = lerp(a[i], b[i], t);
        }
        return out;
    }

   private:
    static uint32_t lerp(uint32_t c1, uint32_t c2, float t) {
        const float u = t < 0.0f ? 0.0f : (t > 1.0f ? 1.0f : t);
        const uint8_t r =
            static_cast<uint8_t>(Color::r(c1) + (Color::r(c2) - Color::r(c1)) * u);
        const uint8_t g =
            static_cast<uint8_t>(Color::g(c1) + (Color::g(c2) - Color::g(c1)) * u);
        const uint8_t b =
            static_cast<uint8_t>(Color::b(c1) + (Color::b(c2) - Color::b(c1)) * u);
        return Color::rgb(r, g, b);
    }
};

}  // namespace pixelengine
