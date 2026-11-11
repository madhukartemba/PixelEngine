#include <PixelEngine.h>

// 10-pixel straight strip (e.g. TankLink-style board)
constexpr uint8_t kPin = 5;
constexpr size_t kCount = 10;

PixelEngine pixels;
uint32_t lastSwitch = 0;
int step = 0;

void setup() {
  pixels.begin(kCount, kPin, Layout::Linear);
  pixels.setFps(60);
  pixels.breathe(Color::Blue, 0.15f, 1.0f);
  lastSwitch = millis();
}

void loop() {
  pixels.update();

  if (millis() - lastSwitch < 4000) {
    return;
  }
  lastSwitch = millis();
  step = (step + 1) % 5;

  switch (step) {
    case 0:
      pixels.breathe(Color::Blue, 0.15f, 1.0f);
      break;
    case 1:
      pixels.spinner(4, Color::Cyan, 1.4f);
      break;
    case 2:
      pixels.wave(Color::Amber, 8.0f);
      break;
    case 3:
      pixels.rainbow(1.0f);
      break;
    default:
      pixels.solid(Color::Green);
      break;
  }
}
