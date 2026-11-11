# ESP-IDF strip example

Use PixelEngine as an IDF component, then:

```cpp
#include "pixelengine/pixel_engine.hpp"

pixelengine::PixelEngine px;

extern "C" void app_main(void) {
  px.begin(10, 5, pixelengine::Layout::Linear);
  px.spinner(4, pixelengine::Color::Blue, 1.2f);
  while (true) {
    px.update();
    vTaskDelay(pdMS_TO_TICKS(1));
  }
}
```

Wire NeoPixels to GPIO 5 (or change the pin argument).
