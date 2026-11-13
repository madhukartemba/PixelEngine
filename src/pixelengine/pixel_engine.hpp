#pragma once

#include <cmath>
#include <memory>

#include "pixelengine/animations/breathe.hpp"
#include "pixelengine/animations/rainbow.hpp"
#include "pixelengine/animations/solid.hpp"
#include "pixelengine/animations/spinner.hpp"
#include "pixelengine/animations/wave.hpp"
#include "pixelengine/clock.hpp"
#include "pixelengine/color.hpp"
#include "pixelengine/engine.hpp"
#include "pixelengine/layout.hpp"

#if defined(ARDUINO)
#include "pixelengine/drivers/neopixel_driver.hpp"
#if defined(FASTLED_VERSION)
#include "pixelengine/drivers/fastled_driver.hpp"
#endif
#endif

#if defined(ESP_PLATFORM) && !defined(ARDUINO)
#include "pixelengine/drivers/esp_led_strip_driver.hpp"
#endif

namespace pixelengine {

enum class Backend {
    Auto,
    NeoPixel,     // Arduino + Adafruit NeoPixel
    FastLED,      // Arduino + FastLED
    EspLedStrip,  // ESP-IDF led_strip
};

/**
 * High-level controller for LED rings and straight strips.
 *
 * Call update() frequently from loop() / a FreeRTOS task.
 */
class PixelEngine {
   public:
    PixelEngine() = default;

    bool begin(size_t led_count, int pin, Layout layout = Layout::Linear,
               Backend backend = Backend::Auto) {
        led_count_ = led_count;
        layout_ = layout;
        std::unique_ptr<LedDriver> driver;

        Backend resolved = backend;
        if (resolved == Backend::Auto) {
#if defined(ESP_PLATFORM) && !defined(ARDUINO)
            resolved = Backend::EspLedStrip;
#else
            resolved = Backend::NeoPixel;
#endif
        }

        switch (resolved) {
#if defined(ARDUINO)
        case Backend::NeoPixel:
            driver = std::make_unique<NeoPixelDriver>(led_count, static_cast<uint8_t>(pin));
            break;
#if defined(FASTLED_VERSION)
        case Backend::FastLED:
            driver = std::make_unique<FastLedDriver>(led_count, static_cast<uint8_t>(pin));
            break;
#endif
#endif
#if defined(ESP_PLATFORM) && !defined(ARDUINO)
        case Backend::EspLedStrip:
            driver = std::make_unique<EspLedStripDriver>(led_count, pin);
            break;
#endif
        default:
            return false;
        }

        engine_ = std::make_unique<AnimationEngine>(std::move(driver));
        last_ms_ = nowMs();
        return true;
    }

    void setLayout(Layout layout) { layout_ = layout; }
    Layout layout() const { return layout_; }

    void setFps(float fps) {
        if (fps < 1.0f) {
            fps = 1.0f;
        }
        frame_interval_ms_ = 1000.0f / fps;
    }

    void update() {
        if (!engine_) {
            return;
        }
        const uint32_t now = nowMs();
        const uint32_t elapsed = now - last_ms_;
        if (elapsed < static_cast<uint32_t>(frame_interval_ms_)) {
            return;
        }
        const float dt = elapsed / 1000.0f;
        last_ms_ = now;
        engine_->update(dt);
    }

    void finishTransition() {
        if (!engine_) {
            return;
        }
        last_ms_ = nowMs();
        while (engine_->fading()) {
            update();
#if defined(ARDUINO)
            delay(1);
#endif
        }
        last_ms_ = nowMs();
    }

    void solid(uint32_t color) { push(std::make_unique<SolidColor>(led_count_, color)); }

    void blank() { solid(Color::Black); }

    void breathe(uint32_t color, float min_brightness = 0.2f, float speed = 1.0f) {
        auto a = std::make_unique<Breathe>(led_count_, color, min_brightness);
        a->speed = speed;
        push(std::move(a));
    }

    void rainbow(float speed = 1.0f, int dir = 1) {
        auto a = std::make_unique<Rainbow>(led_count_);
        a->speed = -std::fabs(speed) * (dir < 0 ? -1.0f : 1.0f);
        push(std::move(a));
    }

    void wave(uint32_t color, float wavelength = 10.0f, int dir = 1) {
        auto a = std::make_unique<Wave>(led_count_, color, wavelength);
        a->speed = -std::fabs(1.0f) * (dir < 0 ? -1.0f : 1.0f);
        push(std::move(a));
    }

    void spinner(size_t trail = 5, uint32_t color = Color::Cyan, float speed = 1.0f,
                 int dir = 1) {
        const bool bounce = (layout_ == Layout::Linear);
        auto a = std::make_unique<Spinner>(led_count_, trail, color, bounce);
        a->speed = std::fabs(speed) * (dir < 0 ? -1.0f : 1.0f);
        push(std::move(a));
    }

    void blink(uint32_t color, uint8_t times = 1) {
        for (uint8_t i = 0; i < times; ++i) {
            push(std::make_unique<SolidColor>(led_count_, color));
            push(std::make_unique<SolidColor>(led_count_, Color::Black));
        }
    }

    void addAnimation(std::unique_ptr<Animation> anim) { push(std::move(anim)); }

    /** Instant swap — used for OTA success/error flashes. */
    void setAnimationImmediate(std::unique_ptr<Animation> anim) {
        if (!engine_ || !anim) {
            return;
        }
        anim->setLayout(layout_);
        engine_->setAnimationImmediate(std::move(anim));
    }

   private:
    void push(std::unique_ptr<Animation> anim) {
        if (!engine_ || !anim) {
            return;
        }
        anim->setLayout(layout_);
        engine_->addAnimation(std::move(anim));
    }

    size_t led_count_ = 0;
    Layout layout_ = Layout::Linear;
    std::unique_ptr<AnimationEngine> engine_;
    uint32_t last_ms_ = 0;
    float frame_interval_ms_ = 1000.0f / 60.0f;
};

}  // namespace pixelengine
