#pragma once

#include <cstdint>

namespace pixelengine {

/** Packed RGB color helpers (0xRRGGBB). Driver-agnostic. */
struct Color {
    static constexpr uint32_t Red = 0xFF0000;
    static constexpr uint32_t Green = 0x00FF00;
    static constexpr uint32_t Blue = 0x0000FF;
    static constexpr uint32_t White = 0xFFFFFF;
    static constexpr uint32_t Black = 0x000000;
    static constexpr uint32_t Yellow = 0xFFFF00;
    static constexpr uint32_t Cyan = 0x00FFFF;
    static constexpr uint32_t Magenta = 0xFF00FF;
    static constexpr uint32_t Orange = 0xFF8000;
    static constexpr uint32_t Amber = 0xFFBF00;
    static constexpr uint32_t Purple = 0x8000FF;
    static constexpr uint32_t Pink = 0xFF0080;
    static constexpr uint32_t Teal = 0x008080;
    static constexpr uint32_t Navy = 0x000080;
    static constexpr uint32_t Gray = 0x808080;

    static constexpr uint32_t rgb(uint8_t r, uint8_t g, uint8_t b) {
        return (uint32_t(r) << 16) | (uint32_t(g) << 8) | uint32_t(b);
    }

    static constexpr uint8_t r(uint32_t c) { return (c >> 16) & 0xFF; }
    static constexpr uint8_t g(uint32_t c) { return (c >> 8) & 0xFF; }
    static constexpr uint8_t b(uint32_t c) { return c & 0xFF; }

    static constexpr uint32_t scale(uint32_t c, float t) {
        if (t <= 0.0f) {
            return Black;
        }
        if (t >= 1.0f) {
            return c;
        }
        return rgb(uint8_t(r(c) * t), uint8_t(g(c) * t), uint8_t(b(c) * t));
    }

    /** HSV → RGB. h in [0, 65535], s/v in [0, 255]. */
    static uint32_t hsv(uint16_t h, uint8_t s = 255, uint8_t v = 255) {
        if (s == 0) {
            return rgb(v, v, v);
        }
        uint8_t region = h / 10923;  // 65536 / 6
        uint16_t rem = (h % 10923) * 6;
        uint8_t p = (v * (255 - s)) >> 8;
        uint8_t q = (v * (255 - ((s * rem) >> 16))) >> 8;
        uint8_t t = (v * (255 - ((s * (65535 - rem)) >> 16))) >> 8;
        switch (region) {
        case 0:
            return rgb(v, t, p);
        case 1:
            return rgb(q, v, p);
        case 2:
            return rgb(p, v, t);
        case 3:
            return rgb(p, q, v);
        case 4:
            return rgb(t, p, v);
        default:
            return rgb(v, p, q);
        }
    }
};

}  // namespace pixelengine
