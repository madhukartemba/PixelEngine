#pragma once

#include <memory>
#include <queue>
#include <vector>

#include "pixelengine/animation.hpp"
#include "pixelengine/combiner.hpp"
#include "pixelengine/driver.hpp"
#include "pixelengine/fader.hpp"

namespace pixelengine {

class AnimationEngine {
   public:
    explicit AnimationEngine(std::unique_ptr<LedDriver> driver) : driver_(std::move(driver)) {
        if (driver_) {
            driver_->begin();
        }
    }

    void addAnimation(std::unique_ptr<Animation> anim) {
        if (!anim) {
            return;
        }
        if (!next_) {
            next_ = std::move(anim);
            next_drawn_ = false;
            fader_.start(0.0f);
        } else {
            queue_.push(std::move(anim));
        }
    }

    void update(float dt) {
        bool dirty = false;

        if (current_ && (!current_->isStatic() || !current_drawn_)) {
            current_->update();
            current_drawn_ = true;
            dirty = true;
        }
        if (next_ && (!next_->isStatic() || !next_drawn_)) {
            next_->update();
            next_drawn_ = true;
            dirty = true;
        }

        std::vector<uint32_t> frame;
        if (next_) {
            dirty = true;
            fader_.update(dt);
            const size_t n = driver_ ? driver_->ledCount() : 0;
            frame = Combiner::blend(current_ ? current_->frame() : std::vector<uint32_t>(n, 0),
                                    next_->frame(), fader_.value());
            if (fader_.complete()) {
                current_ = std::move(next_);
                current_drawn_ = next_drawn_;
                next_.reset();
                next_drawn_ = false;
                if (!queue_.empty()) {
                    next_ = std::move(queue_.front());
                    queue_.pop();
                    next_drawn_ = false;
                    fader_.start(0.0f);
                }
            }
        } else if (current_) {
            frame = current_->frame();
        }

        if (dirty && driver_) {
            driver_->show(frame);
        }
    }

    bool fading() const { return next_ != nullptr || !fader_.complete(); }
    LedDriver* driver() const { return driver_.get(); }

   private:
    std::unique_ptr<LedDriver> driver_;
    std::unique_ptr<Animation> current_;
    std::unique_ptr<Animation> next_;
    std::queue<std::unique_ptr<Animation>> queue_;
    Fader fader_;
    bool current_drawn_ = false;
    bool next_drawn_ = false;
};

}  // namespace pixelengine
