# PixelEngine

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

**Layout-agnostic LED animation engine** for WS2812 / NeoPixel **straight strips** and **rings**.

Evolved from [NeoRing](https://github.com/madhukartemba/NeoRing): same animation ideas, cleaner core, explicit `Linear` vs `Ring` layout, and an **ESP-IDF `led_strip`** backend for projects like TankLink.

---

## Features

| Feature | Detail |
|--------|--------|
| Layouts | `Layout::Linear` (bounce chase) and `Layout::Ring` (wrap) |
| Engine | Non-blocking `update()`, FPS control, crossfade between effects |
| Built-ins | Solid, Breathe, Rainbow, Wave, Spinner, Blink |
| Drivers | Adafruit NeoPixel, FastLED (Arduino), ESP-IDF `led_strip` |
| Extensible | Push custom `Animation` subclasses |

---

## Install

### Arduino IDE

1. Clone or download this repo into your `libraries/` folder as `PixelEngine`
2. Install **Adafruit NeoPixel** from Library Manager
3. `#include <PixelEngine.h>`

### PlatformIO

```ini
lib_deps =
    https://github.com/madhukartemba/PixelEngine.git
    adafruit/Adafruit NeoPixel
```

### ESP-IDF

Add as a component (git submodule or `idf_component.yml`):

```yaml
dependencies:
  madhukartemba/PixelEngine:
    git: https://github.com/madhukartemba/PixelEngine.git
```

Requires `espressif/led_strip`.

---

## Quick start (Arduino strip)

```cpp
#include <PixelEngine.h>

PixelEngine pixels;

void setup() {
  pixels.begin(10, 5, Layout::Linear);  // 10 LEDs on GPIO 5
  pixels.setFps(60);
  pixels.spinner(4, Color::Blue, 1.2f);
}

void loop() {
  pixels.update();
}
```

## Quick start (ring)

```cpp
pixels.begin(24, 6, Layout::Ring);
pixels.rainbow(1.0f);
```

## ESP-IDF

```cpp
#include "pixelengine/pixel_engine.hpp"

pixelengine::PixelEngine px;
px.begin(10, 5, pixelengine::Layout::Linear);  // Auto → EspLedStrip
// In a task: px.update();
```

---

## API sketch

```cpp
pixels.solid(Color::Red);
pixels.breathe(Color::Blue, 0.2f, 1.0f);
pixels.rainbow(1.0f, /*dir=*/1);
pixels.wave(Color::Cyan, /*wavelength=*/8.0f);
pixels.spinner(/*trail=*/5, Color::Magenta, 1.5f);
pixels.blink(Color::White, 3);
pixels.blank();
pixels.addAnimation(std::move(myAnim));
```

---

## Layout behavior

- **Ring** — chase / wave wrap around the loop  
- **Linear** — spinner **bounces** at the ends by default; wave travels along the bar  

---

## Relationship to NeoRing

NeoRing remains the Arduino-oriented ring-first library. PixelEngine is the generalized engine intended for strips + rings and ESP-IDF apps. Prefer PixelEngine for new work.

---

## License

MIT © Madhukar Temba
