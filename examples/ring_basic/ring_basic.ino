#include <PixelEngine.h>

constexpr uint8_t kPin = 6;
constexpr size_t kCount = 24;

PixelEngine pixels;
uint32_t lastSwitch = 0;
int step = 0;

void setup() {
  pixels.begin(kCount, kPin, Layout::Ring);
  pixels.setFps(60);
  pixels.spinner(6, Color::Magenta, 1.2f);
  lastSwitch = millis();
}

void loop() {
  pixels.update();

  if (millis() - lastSwitch < 3500) {
    return;
  }
  lastSwitch = millis();
  step = (step + 1) % 4;

  switch (step) {
    case 0:
      pixels.spinner(6, Color::Magenta, 1.2f);
      break;
    case 1:
      pixels.rainbow(1.0f);
      break;
    case 2:
      pixels.wave(Color::Cyan, 12.0f);
      break;
    default:
      pixels.breathe(Color::White, 0.1f, 0.8f);
      break;
  }
}
