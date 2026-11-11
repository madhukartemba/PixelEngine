#pragma once

namespace pixelengine {

class Fader {
   public:
    void start(float initial = 0.0f, float spd = 1.0f) {
        value_ = initial;
        speed_ = spd;
        active_ = true;
    }

    void update(float dt) {
        if (!active_) {
            return;
        }
        value_ += speed_ * dt;
        if (value_ >= 1.0f) {
            value_ = 1.0f;
            active_ = false;
        }
    }

    float value() const { return value_; }
    bool complete() const { return !active_; }
    void setSpeed(float spd) { speed_ = spd; }

   private:
    float value_ = 0.0f;
    float speed_ = 1.0f;
    bool active_ = false;
};

}  // namespace pixelengine
