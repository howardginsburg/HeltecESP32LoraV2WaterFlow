#include "DisplayUtils.h"

DisplayUtils::DisplayUtils(){
    display = new SSD1306Wire(0x3c, SDA_OLED, SCL_OLED, RST_OLED, GEOMETRY_128_64);
}

DisplayUtils::~DisplayUtils(){
    delete display;
}

void DisplayUtils::init() {
    display->init();
    display->flipScreenVertically();
    display->setFont(ArialMT_Plain_10);
    clear();
}

void DisplayUtils::clear() {
  display->clear();
  for (int i = 0; i < 6; i++) {
    lines[i] = "";
  }
}

void DisplayUtils::print(int line, String text) {
  if (line < 6) {
    lines[line] = text;
  }
  display->clear();
  for (int i = 0; i < 6; i++) {
    display->drawString(0, i * 10, lines[i]);
  }
  display->display();
}

DisplayUtils Display;