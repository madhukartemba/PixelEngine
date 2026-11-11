#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace pixelengine {

/** Hardware backend that displays a packed RGB frame. */
class LedDriver {
   public:
    virtual ~LedDriver() = default;
    virtual void begin() = 0;
    virtual void show(const std::vector<uint32_t>& frame) = 0;
    virtual size_t ledCount() const = 0;
};

}  // namespace pixelengine
