#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>

extern sf::Font font;

extern int menuOffset;

extern bool isPaused;
extern bool isSettings;

void ShowSettings(bool* isOpen);
void ShowMenuBar();
void DrawText(std::string textString, float size, float positionX, float positionY, sf::Color textColor, sf::Font textFont = font);
