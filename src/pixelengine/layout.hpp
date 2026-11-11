#pragma once

#include <cmath>
#include <cstddef>

namespace pixelengine {

/** Physical arrangement of the LED pixels. */
enum class Layout {
    /** Straight strip / bar. Motion can wrap or bounce. */
    Linear,
    /** Closed loop. Motion wraps seamlessly. */
    Ring,
};

/**
 * Map a possibly-out-of-range index into [0, count).
 * Ring always wraps. Linear wraps by default (chase along a bar);
 * use bounce helpers when you want ping-pong motion.
 */
inline size_t wrapIndex(int index, size_t count) {
    if (count == 0) {
        return 0;
    }
    int n = static_cast<int>(count);
    int m = index % n;
    if (m < 0) {
        m += n;
    }
    return static_cast<size_t>(m);
}

inline float wrapFloat(float pos, size_t count) {
    if (count == 0) {
        return 0.0f;
    }
    float n = static_cast<float>(count);
    float m = std::fmod(pos, n);
    if (m < 0.0f) {
        m += n;
    }
    return m;
}

/**
 * Advance a head position. Ring always wraps.
 * Linear with bounce=true reverses at the ends.
 */
inline float advancePosition(float pos, float delta, size_t count, Layout layout, bool* reverse) {
    if (count == 0) {
        return 0.0f;
    }
    float next = pos + delta;
    if (layout == Layout::Ring || reverse == nullptr) {
        return wrapFloat(next, count);
    }
    float max = static_cast<float>(count);
    if (!*reverse) {
        if (next >= max) {
            *reverse = true;
            next = max - (next - max);
        }
    } else {
        if (next < 0.0f) {
            *reverse = false;
            next = -next;
        }
    }
    if (next < 0.0f) {
        next = 0.0f;
    }
    if (next >= max) {
        next = max - 0.001f;
    }
    return next;
}

}  // namespace pixelengine
