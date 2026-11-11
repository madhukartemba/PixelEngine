#pragma once

/**
 * ESP-IDF driver using espressif/led_strip (RMT).
 * Enable by building as an IDF component with led_strip in REQUIRES.
 */
#if defined(ESP_PLATFORM) && !defined(ARDUINO)

#include <algorithm>
#include <cstring>
#include <vector>

#include "esp_log.h"
#include "led_strip.h"
#include "pixelengine/color.hpp"
#include "pixelengine/driver.hpp"

namespace pixelengine {

class EspLedStripDriver : public LedDriver {
   public:
    EspLedStripDriver(size_t led_count, int gpio_num)
        : led_count_(led_count), gpio_(gpio_num) {}

    ~EspLedStripDriver() override {
        if (strip_) {
            led_strip_del(strip_);
            strip_ = nullptr;
        }
    }

    void begin() override {
        led_strip_config_t cfg;
        std::memset(&cfg, 0, sizeof(cfg));
        cfg.strip_gpio_num = gpio_;
        cfg.max_leds = static_cast<uint32_t>(led_count_);

        led_strip_rmt_config_t rmt;
        std::memset(&rmt, 0, sizeof(rmt));
        rmt.resolution_hz = 10 * 1000 * 1000;
        rmt.flags.with_dma = false;

        esp_err_t err = led_strip_new_rmt_device(&cfg, &rmt, &strip_);
        if (err != ESP_OK) {
            ESP_LOGE("PixelEngine", "led_strip init failed: %s", esp_err_to_name(err));
            strip_ = nullptr;
            return;
        }
        led_strip_clear(strip_);
    }

    void show(const std::vector<uint32_t>& frame) override {
        if (!strip_) {
            return;
        }
        const size_t n = std::min(frame.size(), led_count_);
        for (size_t i = 0; i < n; ++i) {
            led_strip_set_pixel(strip_, i, Color::r(frame[i]), Color::g(frame[i]),
                                Color::b(frame[i]));
        }
        led_strip_refresh(strip_);
    }

    size_t ledCount() const override { return led_count_; }

   private:
    size_t led_count_;
    int gpio_;
    led_strip_handle_t strip_ = nullptr;
};

}  // namespace pixelengine

#endif
