#ifndef _DISPLAY_UTILS_h_
#define _DISPLAY_UTILS_h_

#include <Wire.h>
#include "oled/SSD1306Wire.h"

class DisplayUtils {

    public:
        DisplayUtils();
        ~DisplayUtils();
   
        void init();
        void clear();
        void print(int line, String text);
   
    private:
        SSD1306Wire *display;
        String lines[6]; // 6 lines of text with font size 10
};

extern DisplayUtils Display;

#endif // _DISPLAY_UTILS_h_