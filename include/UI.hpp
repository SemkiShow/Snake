#include "raylib.h"
#include "raygui.h"
#include <string>
#include <cmath>

extern bool isPaused;
extern bool isSettings;

int GuiSliderInt(Rectangle bounds, const char *textLeft, const char *textRight, int *value, int minValue, int maxValue);

void DrawSettings(bool* isOpen);
void DrawUI();
