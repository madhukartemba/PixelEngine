#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <vector>

#include "pixelengine/layout.hpp"

namespace pixelengine {

class Animation {
   protected:
    size_t led_count_ = 0;
    Layout layout_ = Layout::Linear;
    bool is_static_ = false;
    std::vector<uint32_t> buffer_;

   public:
    float speed = 1.0f;

    virtual ~Animation() = default;

    void setLayout(Layout layout) { layout_ = layout; }
    Layout layout() const { return layout_; }
    size_t ledCount() const { return led_count_; }

    virtual void update() = 0;
    virtual const std::vector<uint32_t>& frame() const { return buffer_; }
    virtual bool isStatic() const { return is_static_; }

    virtual void reset() { std::fill(buffer_.begin(), buffer_.end(), 0); }
};

}  // namespace pixelengine
